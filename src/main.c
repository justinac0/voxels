// QUESTIONS(justin):
//  What should the renderer do?
//  How should chunks be handled?
//  How can we update individual voxel data efficiently between CPU/GPU
//  How is greedy messhing implemented?
//  How is a sparse octo tree implemented for voxels?
//  What is the thoretical minimum voxel size?
//  How are collisions handled?

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <glad/glad.h>

#include "renderer/glhelp.h"
#include "renderer/shader.h"
#include "chunk/voxel.h"

#define VMATH_HIGH_PREC
#include "math/vmath.h"

bool running = false;

void poll_sdl_events(SDL_Event *event) {
    while (SDL_PollEvent(event)) {
        SDL_KeyboardEvent keyEvent = event->key;
        if (keyEvent.down && keyEvent.key == SDLK_ESCAPE) {
            running = false;
        }
        if (event->type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
}

int main(int argc, char const *argv[]) {
    SDL_Window *window;
    SDL_assert(SDL_Init(SDL_INIT_VIDEO));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "voxels",
        1280,
        720,
        SDL_WINDOW_OPENGL);

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    SDL_assert(glctx != NULL);

    int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_assert(status != 0);

    SDL_assert(SDL_GL_MakeCurrent(window, glctx));

    running = true;

    voxel vox;
    for (int i = 0; i < 10; i++) {
        vox.data[i] = (float)i;
    }

    GLuint shaderProgram = create_shader_program("resources/shaders/triangle.vs", "resources/shaders/triangle.fs");

    // triangle data
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f
    };

    GLuint vaoID = create_vertex_array();
    bind_vertex_array(vaoID);

    GLuint vboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, 9);
    set_vertex_attrib_pointer(0, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(0);

    bind_vertex_array(0);

    mat4x4r perspective = mat4x4r_identity();
    mat4x4r view = mat4x4r_identity();
    mat4x4r model = mat4x4r_identity();

    mat4x4r m;
    m.m00 = m.m11 = m.m22 = m.m33 = 0.5;
    model = mat4x4r_add(&model, &m);

    SDL_Event event;
    while (running) {
        poll_sdl_events(&event);

        glClearColor(0, 0.3, 0.4, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // send uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_projection"), 1, GL_FALSE, (float*)perspective.m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_view"), 1, GL_FALSE, (float*)view.m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_transform"), 1, GL_FALSE, (float*)model.m);

        glUseProgram(shaderProgram);
        bind_vertex_array(vaoID);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(glctx);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}