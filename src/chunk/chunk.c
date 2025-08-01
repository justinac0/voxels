#include "chunk.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
// https://voxelenginetutorial.wiki/greedy-mesh.html

#define CUBE_VERTEX_COUNT (24)
#define VERTEX_COUNT (CHUNK_VOLUME * CUBE_VERTEX_COUNT * 3)
#define INDEX_COUNT (CHUNK_VOLUME * CUBE_VERTS_SIZE)
#define NORMAL_COUNT VERTEX_COUNT
#define TEXTURE_COORD_COUNT VERTEX_COUNT

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

float textureCoords[TEXTURE_COORD_COUNT];
size_t textureIndex = 0;
#define ADD_TEXTURE_COORDS(x, y) textureCoords[textureIndex++] = x; textureCoords[textureIndex++] = y;
#define TEXTURE_COORDS {\
    1.0f, 1.0f,\
    1.0f, 0.0f,\
    0.0f, 0.0f,\
    0.0f, 1.0f\
}

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
    0,1,3,\
    0,3,2,\
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
                voxel->type == VOXEL_TYPE_NONE;
                while (voxel->type == VOXEL_TYPE_NONE) {
                    voxel->type = random_voxel_type();
                }
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

typedef union {
    signed char v[3];
    struct {
        signed char x, y, z;
    };
} Vec3i8;

typedef union {
    unsigned char v[3];
    struct {
        unsigned char x, y, z;
    };
} Vec3u8;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FRONT,
    BACK,

    DIRECTION_COUNT
} Direction;

Vec3i directions[DIRECTION_COUNT] = {};
static inline void set_direction() {
    directions[UP]    = (Vec3i){ 0,  1,  0};
    directions[DOWN]  = (Vec3i){ 0, -1,  0};
    directions[LEFT]  = (Vec3i){-1,  0,  0};
    directions[RIGHT] = (Vec3i){ 1,  0,  0};
    directions[FRONT] = (Vec3i){ 0,  0,  1};
    directions[BACK]  = (Vec3i){ 0,  0, -1};
}

Vec3i get_direction(Direction dir) {
    return directions[dir];
}

typedef struct FaceVertices {
    Vec3r vertices[4]; // 4 vertices per face
} FaceVertices;

FaceVertices face_vertices[DIRECTION_COUNT] = {};
static inline void set_vertices() {
    face_vertices[UP]    = (FaceVertices){ .vertices = TOP_FACE_VERTS };
    face_vertices[DOWN]  = (FaceVertices){ .vertices = BOTTOM_FACE_VERTS };
    face_vertices[LEFT]  = (FaceVertices){ .vertices = LEFT_FACE_VERTS };
    face_vertices[RIGHT] = (FaceVertices){ .vertices = RIGHT_FACE_VERTS };
    face_vertices[FRONT] = (FaceVertices){ .vertices = FRONT_FACE_VERTS };
    face_vertices[BACK]  = (FaceVertices){ .vertices = BACK_FACE_VERTS };
}

FaceVertices get_face_vertices(Direction dir) {
    return face_vertices[dir];
}

typedef struct FaceIndices{
    unsigned int indices[2*3]; // 2 triangles per face, 3 indices per triangle
} FaceIndices;

// 2 triangles per face, 3 indices per triangle
FaceIndices face_indices[DIRECTION_COUNT] = {};
static inline void set_indices() {
    face_indices[UP]    = (FaceIndices)TOP_FACE_INDS;
    face_indices[DOWN]  = (FaceIndices)BOTTOM_FACE_INDS;
    face_indices[LEFT]  = (FaceIndices)LEFT_FACE_INDS;
    face_indices[RIGHT] = (FaceIndices)RIGHT_FACE_INDS;
    face_indices[FRONT] = (FaceIndices)FRONT_FACE_INDS;
    face_indices[BACK]  = (FaceIndices)BACK_FACE_INDS;
}

FaceIndices get_face_indices(Direction dir) {
    return face_indices[dir];
}

void add_voxel_face(Chunk* chunk, Vec3i8 cell, Direction dir) {
    assert(chunk != NULL);

    Voxel* target = chunk_get_voxel(chunk, cell.x, cell.y, cell.z);
    assert(target != NULL);
    
    if (target->type == VOXEL_TYPE_NONE) return;

    Vec3i8 neighbourCell = {
        cell.x + get_direction(dir).x,
        cell.y + get_direction(dir).y,
        cell.z + get_direction(dir).z
    };

    Voxel* neighbour = chunk_get_voxel(chunk, neighbourCell.x, neighbourCell.y, neighbourCell.z);
    if (neighbour != NULL && neighbour->type != VOXEL_TYPE_NONE) return;

    FaceVertices vertices = get_face_vertices(dir);
    FaceIndices indices = get_face_indices(dir);

    size_t indiceOffset = vertexIndex;

    for (size_t i = 0; i < sizeof(vertices.vertices) / sizeof(Vec3r); i++) {
        vertexList[vertexIndex++] = vertices.vertices[i].x + chunk->position.x + cell.x;
        vertexList[vertexIndex++] = vertices.vertices[i].y + chunk->position.y + cell.y;
        vertexList[vertexIndex++] = vertices.vertices[i].z + chunk->position.z + cell.z;

        add_color_vec3r(voxel_color(target->type));
    }

    for (size_t i = 0; i < sizeof(indices.indices) / sizeof(unsigned int); i++) {
        indexList[indexIndex++] = indices.indices[i] + indiceOffset/3;
    }
}

static inline void setup() {
    set_direction();
    set_vertices();
    set_indices();
}

void chunk_make_mesh(Chunk *chunk) {
    setup();

    vertexIndex = 0;
    colourIndex = 0;
    indexIndex = 0;
    textureIndex = 0;

    memset(vertexList, 0, sizeof(vertexList));
    memset(colourList, 0, sizeof(colourList));
    memset(indexList, 0, sizeof(indexList));
    // memset(textureCoords, 0, sizeof(textureCoords));

    for (int8_t x = 0; x < CHUNK_SIZE; x++) {
        for (int8_t y = 0; y < CHUNK_SIZE; y++) {
            for (int8_t z = 0; z < CHUNK_SIZE; z++) {
                Vec3i8 pos = {x, y, z};
                add_voxel_face(chunk, pos, UP);
                add_voxel_face(chunk, pos, DOWN);
                add_voxel_face(chunk, pos, LEFT);
                add_voxel_face(chunk, pos, RIGHT);
                add_voxel_face(chunk, pos, FRONT);
                add_voxel_face(chunk, pos, BACK);
            }
        }
    }

    printf("vertex count: %d (%d)\n", vertexIndex / 3, vertexIndex);
    printf("colour count: %d (%d)\n", colourIndex / 3, colourIndex);
    printf("index count: %d (%d)\n\n", indexIndex / 3, indexIndex);

    chunk->vaoID = create_vertex_array();
    bind_vertex_array(chunk->vaoID);

    chunk->vboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertexList, vertexIndex);
    set_vertex_attrib_pointer(0, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(0);

    chunk->eboID = create_int_buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indexList, indexIndex);

    GLuint cboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, colourList, colourIndex);
    set_vertex_attrib_pointer(1, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(1);

    // GLuint tboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, textureCoords, TEXTURE_COORD_COUNT);
    // set_vertex_attrib_pointer(2, 2, GL_FLOAT, 0, NULL);
    // enable_vertex_attrib_pointer(2);

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
}
