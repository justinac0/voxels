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
#include "renderer/camera.h"
#include "chunk/chunk.h"
#include "input/keyboard.h"
#include "memory/arena.h"

// TODO: common defs
#define KB (1024)
#define MB (KB * 1024)
#define GB (MB * 1024)

#define VMATH_HIGH_PREC
#include "math/vmath.h"

#include "memory/arena.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_TITLE "voxels"

bool running = false;

// void poll_sdl_events(SDL_Event *event) {
//     while (SDL_PollEvent(event)) {
//         switch (event->type) {
//             case SDL_EVENT_QUIT:
//                 running = false;
//                 break;
//         }
//     }
// }

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
        SCREEN_TITLE,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_HideCursor();

    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    SDL_assert(glctx != NULL);

    int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_assert(status != 0);
    SDL_assert(SDL_GL_MakeCurrent(window, glctx));

    running = true;

    GLuint shaderProgram = create_shader_program("resources/shaders/triangle.vs", "resources/shaders/triangle.fs");

    // takes in a position argument
    Arena arena;
    arena_make(&arena, 1 * sizeof(Chunk));
    Chunk *chunk = chunk_create(&arena, vec3r_zero());
    Voxel *vox = chunk_get_voxel(chunk, 15, 15, 15);
    chunk_generate_mesh(chunk);

    printf("voxel size: %lu\n", sizeof(Voxel));
    printf("chunk size: %lu\n", sizeof(Chunk));

    real aspect = 1;

    // NOTE: Adjusting the fov flips the screen (flipped 180* x)?
    real fov = 90*180/3.14;
    Mat4x4r perspective = mat4x4r_perspective(-1, 1, 1, -1, 0.02, 100, fov, aspect);

    Mat4x4r model = mat4x4r_identity();

    SDL_Event event;
    float i = 0;

    Camera camera;
    camera_init(&camera, (Vec3r){0, 0, -3});
    while (running) {
        SDL_PumpEvents();
        SDL_WarpMouseInWindow(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        if (is_key_down(SDLK_ESCAPE)) {
            running = false;
            break;
        }

        if (is_key_down(SDLK_LCTRL)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        float mx, my;
        SDL_GetRelativeMouseState(&mx, &my);
        camera_move(&camera, mx, my);

        // draw
        glClearColor(0.2, 0.4, 0.6, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_projection"), 1, GL_FALSE, (float*)perspective.m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_view"), 1, GL_FALSE, (float*)camera.view.m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_transform"), 1, GL_FALSE, (float*)model.m);

        glUseProgram(shaderProgram);
        chunk_draw(chunk);

        SDL_GL_SwapWindow(window);
    }

    arena_free(&arena);
    SDL_GL_DestroyContext(glctx);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}