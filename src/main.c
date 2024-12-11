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
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_keyboard.h>
#include <glad/glad.h>

#include "renderer/glhelp.h"
#include "renderer/shader.h"
#include "chunk/voxel.h"

#define KB (1024)
#define MB (KB * 1000)
#define GB (MB * 1000)

#define VMATH_HIGH_PREC
#include "math/vmath.h"

#include "memory/arena.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_TITLE "voxels"

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
        if (event->type == SDL_EVENT_WINDOW_MOVED) break;
        if (event->type == SDL_EVENT_WINDOW_RESIZED) break;
        if (event->type == SDL_EVENT_WINDOW_FOCUS_GAINED) break;
    }
}

int main(int argc, char const *argv[]) {
    Arena arena;
    arena_make(&arena, (size_t)(30) * KB);

    unsigned char* numbers = arena_alloc(&arena, 4098 * sizeof(unsigned char));
    int* bigNumbers = arena_alloc(&arena, 1000 * sizeof(int));
    float* reals = arena_alloc(&arena, 2000 * sizeof(float));
    short* shorts = arena_alloc(&arena, 5000 * sizeof(short));

    arena_free(&arena);

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
        SCREEN_TITLE,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
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

    real aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
    Mat4x4r perspective = mat4x4r_perspective(-1, 1, 1, -1, 0.02, 100, 90, aspect);

    Mat4x4r model = mat4x4r_identity();

    Vec3r position = {0, 0, -2};
    SDL_Event event;
    float i = 0;
    while (running) {
        poll_sdl_events(&event);

        i += 0.001;
        position.y = sinf(i);
        position.x = cosf(i);

        Mat4x4r view = mat4x4r_lookat_col(vec3r_right(), vec3r_up(), vec3r_forward(), position);

        glClearColor(0.2, 0.4, 0.6, 1);
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