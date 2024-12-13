#ifndef CHUNK_H
#define CHUNK_H

#include <stdio.h>
#include <stdint.h>

#include "../renderer/glhelp.h"
#include "../math/vec3r.h"
#include "../memory/arena.h"
#include "voxel.h"

#define CHUNK_SIZE 32
#define CHUNK_AREA (CHUNK_SIZE*CHUNK_SIZE)
#define CHUNK_VOLUME (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)
#define CUBE_VERTS_SIZE (36)
#define CUBE_SIZE_SCALING 0.4

// Normal calculations: https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
typedef struct {
    Voxel voxels[CHUNK_VOLUME];
    Vec3r position; // the smallest x, y, z value of the chunk
    GLuint vaoID;
    GLuint eboID;
} Chunk;

Chunk *chunk_create(Arena *arena, Vec3r position);
size_t chunk_get_voxel_index(uint8_t x, uint8_t y, uint8_t z);
Voxel *chunk_get_voxel(Chunk *chunk, uint8_t x, uint8_t y, uint8_t z);
void chunk_generate_mesh(Chunk *chunk);
void chunk_draw(Chunk *chunk);

#endif // CHUNK_H