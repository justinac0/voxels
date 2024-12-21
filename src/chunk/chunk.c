#include "chunk.h"

// https://voxelenginetutorial.wiki/greedy-mesh.html

#define CUBE_VERTEX_COUNT (24)
#define VERTEX_COUNT (CHUNK_VOLUME * CUBE_VERTEX_COUNT * 3)
#define INDEX_COUNT (CHUNK_VOLUME * CUBE_VERTS_SIZE)
#define NORMAL_COUNT VERTEX_COUNT

float vertexList[VERTEX_COUNT];
size_t vertexIndex = 0;
#define ADD_VERTEX(x, y, z) vertexList[vertexIndex++] = x; vertexList[vertexIndex++] = y; vertexList[vertexIndex++] = z;

float colourList[VERTEX_COUNT];
size_t colourIndex = 0;
#define ADD_COLOUR(r, g, b) colourList[colourIndex++] = r; colourList[colourIndex++] = g; colourList[colourIndex++] = b;

static inline void add_color_vec3r(Vec3r colour) {
    colourList[colourIndex++] = colour.r; colourList[colourIndex++] = colour.g; colourList[colourIndex++] = colour.b;
}

unsigned int indexList[INDEX_COUNT];
size_t indexIndex = 0;
#define ADD_TRIANGLE(a, b, c) indexList[indexIndex++] = a; indexList[indexIndex++] = b; indexList[indexIndex++] = c;

#define FRONT_FACE_VERTS {\
    -0.5,  0.5, 0.5,\
     0.5,  0.5, 0.5,\
    -0.5, -0.5, 0.5,\
     0.5, -0.5, 0.5,\
}
#define FRONT_FACE_INDS {\
    0,1,3,\
    0,3,2,\
}
#define BACK_FACE_VERTS {\
    -0.5,  0.5,  -0.5,\
     0.5,  0.5,  -0.5,\
    -0.5, -0.5,  -0.5,\
     0.5, -0.5,  -0.5,\
}
#define BACK_FACE_INDS {\
    0,3,1,\
    0,2,3,\
}
#define LEFT_FACE_VERTS {\
    -0.5,  0.5, -0.5,\
    -0.5,  0.5,  0.5,\
    -0.5, -0.5, -0.5,\
    -0.5, -0.5,  0.5,\
}
#define LEFT_FACE_INDS {\
    0,3,1,\
    0,2,3,\
}
#define RIGHT_FACE_VERTS {\
    0.5,  0.5, -0.5,\
    0.5,  0.5,  0.5,\
    0.5, -0.5, -0.5,\
    0.5, -0.5,  0.5,\
}
#define RIGHT_FACE_INDS {\
    0,3,1,\
    0,2,3,\
}
#define TOP_FACE_VERTS {\
    -0.5,  0.5,  0.5,\
     0.5,  0.5,  0.5,\
    -0.5,  0.5, -0.5,\
     0.5,  0.5, -0.5,\
}
#define TOP_FACE_INDS {\
    0,3,1,\
    0,2,3,\
}
#define BOTTOM_FACE_VERTS {\
    -0.5, -0.5,  0.5,\
     0.5, -0.5,  0.5,\
    -0.5, -0.5, -0.5,\
     0.5, -0.5, -0.5\
}
#define BOTTOM_FACE_INDS {\
    0,1,3,\
    0,3,2,\
}

size_t chunk_get_index(int8_t x, int8_t y, int8_t z) {
    return x * CHUNK_AREA + y * CHUNK_SIZE + z;
}

Voxel *chunk_get_voxel(Chunk *chunk, int8_t x, int8_t y, int8_t z) {
    if (x < 0 || x >= CHUNK_SIZE ||
        y < 0 || y >= CHUNK_SIZE ||
        z < 0 || z >= CHUNK_SIZE) {
        return NULL;
    }

    return &chunk->voxels[chunk_get_index(x, y, z)];
}

Chunk *chunk_create(Arena *arena, Vec3r position) {
    srand(time(NULL));
    Chunk *chunk = arena_alloc(arena, sizeof(Chunk));

    if (chunk == NULL) return NULL;
    chunk->position = position;

    return chunk;
}

VoxelType random_voxel_type() {
    return rand() % VOXEL_TYPE_COUNT;
}

void chunk_generate(Chunk *chunk) {
    for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
        for (uint8_t y = 0; y < CHUNK_SIZE; y++) {
            for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
                Voxel* voxel = chunk_get_voxel(chunk, x, y, z);
                voxel->type = random_voxel_type();
            }
        }
    }
}

typedef union {
    unsigned int v[3];
    struct {
        unsigned int x, y, z;
    };
} Vec3i;

void add_face(Vec3r* chunkPos, Vec3r* centerPos, Voxel* center) {
    Vec3r color = voxel_color(center->type);
    Vec3r verts[] = LEFT_FACE_VERTS;
    unsigned int idx[] = FRONT_FACE_INDS;

    // record where the indices for this face start
    size_t indiceOffset = vertexIndex;

    float x = chunkPos->x + centerPos->x;
    float y = chunkPos->y + centerPos->y;
    float z = chunkPos->z + centerPos->z;

    for (size_t i = 0; i < sizeof(verts) / sizeof(Vec3r); i++) {
        ADD_VERTEX(verts[i].x + x, verts[i].y + y, verts[i].z + z);
        ADD_COLOUR(color.r, color.g, color.b);
    }
    for (int i = 0; i < 2; i++) {
        ADD_TRIANGLE(idx[i*3] + indiceOffset/3, idx[i*3+1] + indiceOffset/3, idx[i*3+2] + indiceOffset/3);
    }
}

void generate_cube_faces(Vec3r* chunkPos, Vec3r* centerPos, Voxel* center, Voxel* top, Voxel* bottom, Voxel* left, Voxel* right, Voxel* front, Voxel* back) {
    if (center->type == VOXEL_TYPE_NONE) return;

    // TODO: fill in vertices, color, normals : vert index winding order (Counter Clockwise)
    Vec3r color = voxel_color(center->type);
    if (!top || top->type == VOXEL_TYPE_NONE) {
        Vec3r verts[] = TOP_FACE_VERTS;
        unsigned int idx[] = TOP_FACE_INDS;

        // record where the indices for this face start
        size_t indiceOffset = vertexIndex;

        float x = chunkPos->x + centerPos->x;
        float y = chunkPos->y + centerPos->y;
        float z = chunkPos->z + centerPos->z;

        for (size_t i = 0; i < sizeof(verts) / sizeof(Vec3r); i++) {
            ADD_VERTEX(verts[i].x + x, verts[i].y + y, verts[i].z + z);
            add_color_vec3r(voxel_color(VOXEL_TYPE_RED));
        }
        for (int i = 0; i < 2; i++) {
            ADD_TRIANGLE(idx[i*3] + indiceOffset/3, idx[i*3+1] + indiceOffset/3, idx[i*3+2] + indiceOffset/3);
        }
    }
    if (!bottom || bottom->type == VOXEL_TYPE_NONE) {
        Vec3r verts[] = BOTTOM_FACE_VERTS;
        unsigned int idx[] = BOTTOM_FACE_INDS;

        // record where the indices for this face start
        size_t indiceOffset = vertexIndex;

        float x = chunkPos->x + centerPos->x;
        float y = chunkPos->y + centerPos->y;
        float z = chunkPos->z + centerPos->z;

        for (size_t i = 0; i < sizeof(verts) / sizeof(Vec3r); i++) {
            ADD_VERTEX(verts[i].x + x, verts[i].y + y, verts[i].z + z);
            add_color_vec3r(voxel_color(VOXEL_TYPE_GREEN));
        }
        for (int i = 0; i < 2; i++) {
            ADD_TRIANGLE(idx[i*3] + indiceOffset/3, idx[i*3+1] + indiceOffset/3, idx[i*3+2] + indiceOffset/3);
        }
    }
    if (!left || left->type == VOXEL_TYPE_NONE) {
        Vec3r verts[] = LEFT_FACE_VERTS;
        unsigned int idx[] = FRONT_FACE_INDS;

        // record where the indices for this face start
        size_t indiceOffset = vertexIndex;

        float x = chunkPos->x + centerPos->x;
        float y = chunkPos->y + centerPos->y;
        float z = chunkPos->z + centerPos->z;

        for (size_t i = 0; i < sizeof(verts) / sizeof(Vec3r); i++) {
            ADD_VERTEX(verts[i].x + x, verts[i].y + y, verts[i].z + z);
            add_color_vec3r(voxel_color(VOXEL_TYPE_BLUE));
        }
        for (int i = 0; i < 2; i++) {
            ADD_TRIANGLE(idx[i*3] + indiceOffset/3, idx[i*3+1] + indiceOffset/3, idx[i*3+2] + indiceOffset/3);
        }
    }
    if (!right || right->type == VOXEL_TYPE_NONE) {
        Vec3r verts[] = RIGHT_FACE_VERTS;
        unsigned int idx[] = RIGHT_FACE_INDS;

        // record where the indices for this face start
        size_t indiceOffset = vertexIndex;

        float x = chunkPos->x + centerPos->x;
        float y = chunkPos->y + centerPos->y;
        float z = chunkPos->z + centerPos->z;

        for (size_t i = 0; i < sizeof(verts) / sizeof(Vec3r); i++) {
            ADD_VERTEX(verts[i].x + x, verts[i].y + y, verts[i].z + z);
            add_color_vec3r(voxel_color(VOXEL_TYPE_GRAY));
        }
        for (int i = 0; i < 2; i++) {
            ADD_TRIANGLE(idx[i*3] + indiceOffset/3, idx[i*3+1] + indiceOffset/3, idx[i*3+2] + indiceOffset/3);
        }
    }
    if (!front || front->type == VOXEL_TYPE_NONE) {
        Vec3r verts[] = FRONT_FACE_VERTS;
        unsigned int idx[] = FRONT_FACE_INDS;

        // record where the indices for this face start
        size_t indiceOffset = vertexIndex;

        float x = chunkPos->x + centerPos->x;
        float y = chunkPos->y + centerPos->y;
        float z = chunkPos->z + centerPos->z;

        for (size_t i = 0; i < sizeof(verts) / sizeof(Vec3r); i++) {
            ADD_VERTEX(verts[i].x + x, verts[i].y + y, verts[i].z + z);
            add_color_vec3r(voxel_color(VOXEL_TYPE_CYAN));
        }
        for (int i = 0; i < 2; i++) {
            ADD_TRIANGLE(idx[i*3] + indiceOffset/3, idx[i*3+1] + indiceOffset/3, idx[i*3+2] + indiceOffset/3);
        }
    }
    if (!back || back->type == VOXEL_TYPE_NONE) {
        Vec3r verts[] = BACK_FACE_VERTS;
        unsigned int idx[] = BACK_FACE_INDS;

        // record where the indices for this face start
        size_t indiceOffset = vertexIndex;

        float x = chunkPos->x + centerPos->x;
        float y = chunkPos->y + centerPos->y;
        float z = chunkPos->z + centerPos->z;

        for (size_t i = 0; i < sizeof(verts) / sizeof(Vec3r); i++) {
            ADD_VERTEX(verts[i].x + x, verts[i].y + y, verts[i].z + z);
            add_color_vec3r(voxel_color(VOXEL_TYPE_BROWN));
        }
        for (int i = 0; i < 2; i++) {
            ADD_TRIANGLE(idx[i*3] + indiceOffset/3, idx[i*3+1] + indiceOffset/3, idx[i*3+2] + indiceOffset/3);
        }
    }
}

void chunk_make_mesh2(Chunk *chunk) {
    for (int8_t x = 0; x < CHUNK_SIZE; x++) {
        for (int8_t y = 0; y < CHUNK_SIZE; y++) {
            for (int8_t z = 0; z < CHUNK_SIZE; z++) {
                Voxel *voxel = chunk_get_voxel(chunk, x, y, z);
                Voxel *top = chunk_get_voxel(chunk, x, y+1, z);
                Voxel *bottom = chunk_get_voxel(chunk, x, y-1, z);
                Voxel *left = chunk_get_voxel(chunk, x-1, y, z);
                Voxel *right = chunk_get_voxel(chunk, x+1, y, z);
                Voxel *front = chunk_get_voxel(chunk, x, y, z+1);
                Voxel *back = chunk_get_voxel(chunk, x, y, z-1);

                Vec3r pos = {x,y,z};
                generate_cube_faces(&chunk->position, &pos, voxel, top, bottom, left, right, front, back);
            }
        }
    }

    printf("vertex count: %d (%d)\n", vertexIndex / 3, vertexIndex);
    printf("colour count: %d (%d)\n", colourIndex / 3, colourIndex);
    printf("index count: %d (%d)\n", indexIndex / 3, indexIndex);

    chunk->vaoID = create_vertex_array();
    bind_vertex_array(chunk->vaoID);

    chunk->vboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertexList, vertexIndex);
    set_vertex_attrib_pointer(0, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(0);

    chunk->eboID = create_int_buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indexList, indexIndex);

    GLuint cboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, colourList, colourIndex);
    set_vertex_attrib_pointer(1, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(1);


    bind_vertex_array(0);
}

void chunk_free_mesh(Chunk *chunk) {
    glDeleteBuffers(1, &chunk->eboID);
    glDeleteBuffers(1, &chunk->vboID);
    glDeleteVertexArrays(1, &chunk->vaoID);
}

void chunk_draw(Chunk *chunk) {
    bind_vertex_array(chunk->vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->eboID);
    glDrawElements(GL_TRIANGLES, indexIndex, GL_UNSIGNED_INT, 0);
    // printf("idx: %d\n", indexIndex);
}
