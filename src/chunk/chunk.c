#include "chunk.h"

#include <stdio.h>

Chunk *chunk_create(Arena *arena, Vec3r position) {
    Chunk *chunk = arena_alloc(arena, sizeof(Chunk));

    if (chunk == NULL) return NULL;

    chunk->position = position;
    for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
        for (uint8_t y = 0; y < CHUNK_SIZE; y++) {
            for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
                Voxel* voxel = chunk_get_voxel(chunk, x, y, z);
                voxel->type = VOXEL_TYPE_BLACK;
            }
        }
    }

    return chunk;
}

size_t chunk_get_index(uint8_t x, uint8_t y, uint8_t z) {
    return x * CHUNK_AREA + y * CHUNK_SIZE + z;
}

Voxel *chunk_get_voxel(Chunk *chunk, uint8_t x, uint8_t y, uint8_t z) {
    if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
        return NULL;
    }

    return &chunk->voxels[chunk_get_index(x, y, z)];
}

#define CUBE_VERTS { \
    -0.5, -0.5, -0.5, \
     0.5, -0.5, -0.5, \
     0.5,  0.5, -0.5, \
    -0.5,  0.5, -0.5, \
    -0.5, -0.5,  0.5, \
     0.5, -0.5,  0.5, \
     0.5,  0.5,  0.5, \
    -0.5,  0.5,  0.5 \
}

#define CUBE_IDXS { \
    4, 5, 6, \
    6, 7, 4, \
    0, 1, 2, \
    2, 3, 0, \
    0, 4, 7, \
    7, 3, 0, \
    1, 5, 6, \
    6, 2, 1, \
    3, 7, 6, \
    6, 2, 3, \
    0, 4, 5, \
    5, 1, 0 \
}

#define CUBE_SIZE_SCALING 0.4

void add_cube_vertices(Vec3r *vertices, size_t *index, uint8_t x, uint8_t y, uint8_t z) {
    Vec3r cubeVertices[] = CUBE_VERTS;
    Vec3r offset = {x, y, z};

    for (size_t i = 0; i < 8; i++) {
        cubeVertices[i] = vec3r_scalar(&cubeVertices[i], CUBE_SIZE_SCALING);
        vertices[(*index)++] = vec3r_add(&cubeVertices[i], &offset);
    }
}

typedef union {
    unsigned int v[3];
    struct {
        unsigned int x, y, z;
    };
} Vec3i;

void add_cube_indices(Vec3i *indices, size_t *index, uint8_t x, uint8_t y, uint8_t z) {
    Vec3i cubeIndices[] = CUBE_IDXS;
    size_t offset = chunk_get_index(x, y, z);

    for (size_t i = 0; i < 12; i++) {
        cubeIndices[i].x += offset * 8;
        cubeIndices[i].y += offset * 8;
        cubeIndices[i].z += offset * 8;

        indices[(*index)++] = cubeIndices[i];
    }
}

// #define CUBE_VERTICES(x, y, z, size) { \
//     -size + x, -size + y, -size + z, \
//      size + x, -size + y, -size + z, \
//      size + x,  size + y, -size + z, \
//     -size + x,  size + y, -size + z, \
//     -size + x, -size + y,  size + z, \
//      size + x, -size + y,  size + z, \
//      size + x,  size + y,  size + z, \
//     -size + x,  size + y,  size + z \
// }

// #define CUBE_INDICES(i) { \
//     4+(i*8), 5+(i*8), 6+(i*8), \
//     6+(i*8), 7+(i*8), 4+(i*8), \
//     0+(i*8), 1+(i*8), 2+(i*8), \
//     2+(i*8), 3+(i*8), 0+(i*8), \
//     0+(i*8), 4+(i*8), 7+(i*8), \
//     7+(i*8), 3+(i*8), 0+(i*8), \
//     1+(i*8), 5+(i*8), 6+(i*8), \
//     6+(i*8), 2+(i*8), 1+(i*8), \
//     3+(i*8), 7+(i*8), 6+(i*8), \
//     6+(i*8), 2+(i*8), 3+(i*8), \
//     0+(i*8), 4+(i*8), 5+(i*8), \
//     5+(i*8), 1+(i*8), 0+(i*8)  \
// }

#define VERTEX_COUNT (CHUNK_VOLUME * 8 * 3)
#define INDEX_COUNT (CHUNK_VOLUME * 6 * 2 * 3)

// 8 vertices per cube, with 3 floats (xyz) per vertex
float vertexList[VERTEX_COUNT];
// 6 faces per cube, with 2 triangles per cube face (square), with 3 vertices per triangle
unsigned int indexList[INDEX_COUNT];

void chunk_generate_mesh(Chunk *chunk) {
    size_t vertexIndex = 0;
    size_t indexIndex = 0;

    // chunk->voxels[];

    for (size_t x = 0; x < CHUNK_SIZE; x++) {
        for (size_t y = 0; y < CHUNK_SIZE; y++) {
            for (size_t z = 0; z < CHUNK_SIZE; z++) {
                add_cube_vertices((Vec3r *) vertexList, &vertexIndex, x, y, z);
                add_cube_indices((Vec3i *) indexList, &indexIndex, x, y, z);
            }
        }
    }

    printf("vertex count: %d\n", vertexIndex);
    printf("index count: %d\n", indexIndex);

    chunk->vaoID = create_vertex_array();
    bind_vertex_array(chunk->vaoID);

    GLuint vboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertexList, VERTEX_COUNT);
    set_vertex_attrib_pointer(0, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(0);

    chunk->eboID = create_int_buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indexList, INDEX_COUNT);

    bind_vertex_array(0);
}

void chunk_draw(Chunk *chunk) {
    bind_vertex_array(chunk->vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->eboID);
    glDrawElements(GL_TRIANGLES, 36*CHUNK_VOLUME, GL_UNSIGNED_INT, 0);
}