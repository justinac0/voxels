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

    float size = 0.5;

    float vertices[] = {
        // Positions
        -1.0f, -1.0f, -1.0f,  // 0: Bottom-back-left
        1.0f, -1.0f, -1.0f,  // 1: Bottom-back-right
        1.0f,  1.0f, -1.0f,  // 2: Top-back-right
        -1.0f,  1.0f, -1.0f,  // 3: Top-back-left
        -1.0f, -1.0f,  1.0f,  // 4: Bottom-front-left
        1.0f, -1.0f,  1.0f,  // 5: Bottom-front-right
        1.0f,  1.0f,  1.0f,  // 6: Top-front-right
        -1.0f,  1.0f,  1.0f   // 7: Top-front-left
    };

    unsigned int indices[] = {
        // Front face
        4, 5, 6,
        6, 7, 4,

        // Back face
        0, 1, 2,
        2, 3, 0,

        // Left face
        0, 4, 7,
        7, 3, 0,

        // Right face
        1, 5, 6,
        6, 2, 1,

        // Top face
        3, 7, 6,
        6, 2, 3,

        // Bottom face
        0, 4, 5,
        5, 1, 0
    };

    chunk->vaoID = create_vertex_array();
    bind_vertex_array(chunk->vaoID);

    GLuint vboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
    set_vertex_attrib_pointer(0, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(0);

    chunk->eboID = create_int_buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));

    bind_vertex_array(0);
}

void chunk_draw(Chunk *chunk) {
    bind_vertex_array(chunk->vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->eboID);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}