#ifndef CHUNK_H
#define CHUNK_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../renderer/glhelp.h"
#include "../math/vec3r.h"
#include "../memory/arena.h"
#include "voxel.h"

#define CHUNK_SIZE (16)
#define CHUNK_AREA (CHUNK_SIZE*CHUNK_SIZE)
#define CHUNK_VOLUME (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)
#define CUBE_VERTS_SIZE (36)
#define CUBE_SIZE_SCALING (0.1)

// Normal calculations: https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
typedef struct Chunk {
    Voxel voxels[CHUNK_VOLUME];
    Vec3r position;
    GLuint vaoID;
    GLuint vboID;
    GLuint nboID;
    GLuint eboID;
} Chunk;

Chunk *chunk_create(Arena *arena, Vec3r position);
void chunk_generate(Chunk *chunk);
size_t chunk_get_voxel_index(int8_t x, int8_t y, int8_t z);
Voxel *chunk_get_voxel(Chunk *chunk, int8_t x, int8_t y, int8_t z);
void chunk_make_mesh(Chunk *chunk);
void chunk_free_mesh(Chunk *chunk);
void chunk_draw(Chunk *chunk);

#endif // CHUNK_H
