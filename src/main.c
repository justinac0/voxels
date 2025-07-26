#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_keyboard.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "renderer/glhelp.h"
#include "renderer/shader.h"
#include "renderer/camera.h"
#include "chunk/chunk.h"
#include "input/keyboard.h"
#include "memory/arena.h"

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

    // seed random number generator
    srand(time(NULL));

    GLuint shaderProgram = create_shader_program("resources/shaders/triangle.vs", "resources/shaders/triangle.fs");

    // takes in a position argument
    int chunk_count = 10;
    Arena chunkArena;
    arena_make(&chunkArena, chunk_count*sizeof(Chunk));
    Chunk *chunks[chunk_count];
    for (int i = 0; i < chunk_count; i++) {
        chunks[i] = chunk_create(&chunkArena, (Vec3r){i*CHUNK_SIZE, 0, i*CHUNK_SIZE});
        chunk_generate(chunks[i]);
        chunk_make_mesh(chunks[i]);
    }

    printf("cs: %lu\n", sizeof(Chunk));

    real aspect = 1.75f;
    real fov = DEG2RAD(70);

    Mat4x4r perspective = mat4x4r_perspective(-1, 1, 1, -1, 0.1, 100.0, fov, aspect);
    Mat4x4r model = mat4x4r_identity();

    // TODO: front face should be clockwise winding
    //       back faces should be culled...
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);

    SDL_Event event;
    Camera camera;
    camera_init(&camera, (Vec3r){0, 0, -32});
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
        glClearColor(0.2,0.2,0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_projection"), 1, GL_FALSE, (float*)perspective.m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_view"), 1, GL_FALSE, (float*)camera.view.m);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "m_transform"), 1, GL_FALSE, (float*)model.m);

        glUseProgram(shaderProgram);
        for (int i = 0; i < chunk_count; i++) {
            chunk_draw(chunks[i]);
        }

        SDL_GL_SwapWindow(window);
    }

    arena_free(&chunkArena);
    SDL_GL_DestroyContext(glctx);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
