#include "chunk.h"

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

Voxel *chunk_get_voxel(Chunk *chunk, uint8_t x, uint8_t y, uint8_t z) {
    if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
        return NULL;
    }

    return &chunk->voxels[x * CHUNK_AREA + y * CHUNK_SIZE + z];
}

void chunk_generate_mesh(Chunk *chunk) {
    chunk->vaoID = create_vertex_array();
    bind_vertex_array(chunk->vaoID);

    float vertices[] = {
        -1f, -1f, 1f,
        1f, -1f, 1f,
        1f, 1f, 1f,
        -1f, 1f, 1f,

        -1f, -1f, -1f,
        1f, -1f, -1f,
        1f, 1f, -1f,
        -1f, 1f, -1f
    };

    int indicies[] {
        0, 1, 2,
        2, 3, 0,

        1, 5, 6,
        6, 2, 1,

        7, 6, 5,
        5, 4, 7,

        4, 0, 3,
        3, 7, 4,

        4, 5, 1,
        1, 0, 4,

        3, 2, 6,
        6, 7, 3
    };

    GLuint iboID = create_int_buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, 0);
    GLuint vboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, 8);
    set_vertex_attrib_pointer(0, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(0);

    bind_vertex_array(0);
}

void chunk_draw(Chunk *chunk) {
    bind_vertex_array(chunk->vaoID);
    glDrawElements(GL_TRIANGLES. 36, GL_UNSIGNED_SHORT, NULL);
}