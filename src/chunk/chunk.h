#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include "../renderer/glhelp.h"
#include "../math/vec3r.h"
#include "../memory/arena.h"
#include "voxel.h"

#define CHUNK_SIZE 1 // 16
#define CHUNK_AREA (CHUNK_SIZE*CHUNK_SIZE)
#define CHUNK_VOLUME (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)

// Chunk Slice
// 0 0, 0 1, 0 2 ...
// 1 0, 1 1, 1 2 ...
// 2 0, 2 1, 2 2 ...
// ...
typedef struct {
    Voxel voxels[CHUNK_VOLUME];
    Vec3r position; // the smallest x, y, z value of the chunk
    GLuint vaoID;
    GLuint eboID;
} Chunk;

// Generate Chunk Mesh
// VAO - Render Call
//   Chunk
//   Voxels
//    VBO
//    Mesh (instanced?)
//     Cube Verticies - indicies
//     Normals
//     Color/Texture

// Chunk Draw
//  Draws from VAO

Chunk *chunk_create(Arena *arena, Vec3r position);
Voxel *chunk_get_voxel(Chunk *chunk, uint8_t x, uint8_t y, uint8_t z);
void chunk_generate_mesh(Chunk *chunk);
void chunk_draw(Chunk *chunk);

#endif // CHUNK_H