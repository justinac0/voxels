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

#define CUBE_VERTICES(x, y, z) { \
    -1.0f + x, -1.0f + y, -1.0f + z, \
     1.0f + x, -1.0f + y, -1.0f + z, \
     1.0f + x,  1.0f + y, -1.0f + z, \
    -1.0f + x,  1.0f + y, -1.0f + z, \
    -1.0f + x, -1.0f + y,  1.0f + z, \
     1.0f + x, -1.0f + y,  1.0f + z, \
     1.0f + x,  1.0f + y,  1.0f + z, \
    -1.0f + x,  1.0f + y,  1.0f + z \
}

#define CUBE_INDICES(i) { \
    4+(i*8), 5+(i*8), 6+(i*8), \
    6+(i*8), 7+(i*8), 4+(i*8), \
    0+(i*8), 1+(i*8), 2+(i*8), \
    2+(i*8), 3+(i*8), 0+(i*8), \
    0+(i*8), 4+(i*8), 7+(i*8), \
    7+(i*8), 3+(i*8), 0+(i*8), \
    1+(i*8), 5+(i*8), 6+(i*8), \
    6+(i*8), 2+(i*8), 1+(i*8), \
    3+(i*8), 7+(i*8), 6+(i*8), \
    6+(i*8), 2+(i*8), 3+(i*8), \
    0+(i*8), 4+(i*8), 5+(i*8), \
    5+(i*8), 1+(i*8), 0+(i*8)  \
}

void chunk_generate_mesh(Chunk *chunk) {
    chunk->vaoID = create_vertex_array();
    bind_vertex_array(chunk->vaoID);

    float size = 0.5;

#define MAX_VERTEX_COUNT CHUNK_VOLUME*8

    float vertices[] = CUBE_VERTICES(0, 0, 0);
    unsigned int indices[] = CUBE_INDICES(0);

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