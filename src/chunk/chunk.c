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

unsigned int indexList[INDEX_COUNT];
size_t indexIndex = 0;
#define ADD_TRIANGLE(a, b, c) indexList[indexIndex++] = a; indexList[indexIndex++] = b; indexList[indexIndex++] = c;

#define FRONT_FACE_VERTS {\
    -0.5,  0.5, -0.5,\
     0.5,  0.5, -0.5,\
    -0.5, -0.5, -0.5,\
     0.5, -0.5, -0.5,\
}
#define FRONT_FACE_INDS {\
    0,1,3,\
    0,3,2,\
}
#define BACK_FACE_VERTS {\
    -0.5,  0.5,  0.5,\
     0.5,  0.5,  0.5,\
    -0.5, -0.5,  0.5,\
     0.5, -0.5,  0.5,\
}
#define BACK_FACE_INDS {\
    0,3,1,\
    0,2,3,\
}
#define LEFT_FACE_VERTS {\
    0.5,  0.5, -0.5,\
    0.5,  0.5,  0.5,\
    0.5, -0.5, -0.5,\
    0.5, -0.5,  0.5,\
}
#define LEFT_FACE_INDS {\
    0,3,1,\
    0,2,3,\
}

// float normalList[VERTEX_COUNT];
// size_t normalIndex = 0;

// ref: https://community.khronos.org/t/cube-with-indices/105329/2
// https://github.com/lszl84/wx_gl_cube_tutorial/blob/main/src/cube.h
#define CUBE_VERTS {\
    -0.5,  0.5, -0.5,\
     0.5,  0.5, -0.5,\
    -0.5, -0.5, -0.5,\
     0.5, -0.5, -0.5,\
    \
    -0.5,  0.5,  0.5,\
     0.5,  0.5,  0.5,\
    -0.5, -0.5,  0.5,\
     0.5, -0.5,  0.5,\
    \
     0.5,  0.5, -0.5,\
     0.5,  0.5,  0.5,\
     0.5, -0.5, -0.5,\
     0.5, -0.5,  0.5,\
    \
    -0.5,  0.5, -0.5,\
    -0.5,  0.5,  0.5,\
    -0.5, -0.5, -0.5,\
    -0.5, -0.5,  0.5,\
    \
    -0.5,  0.5,  0.5,\
     0.5,  0.5,  0.5,\
    -0.5,  0.5, -0.5,\
     0.5,  0.5, -0.5,\
    \
    -0.5, -0.5,  0.5,\
     0.5, -0.5,  0.5,\
    -0.5, -0.5, -0.5,\
     0.5, -0.5, -0.5\
}

#define CUBE_IDXS {\
    0,3,1,\
    0,2,3,\
    \
    7,6,4,\
    5,7,4,\
    \
    8,11,9,\
    8,10,11,\
    \
    12,15,14,\
    12,13,15,\
    \
    16,18,19,\
    16,19,17,\
    \
    20,21,23,\
    20,23,22\
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
                // voxel->type = ((x + y + z) % 2 == 0) ? VOXEL_TYPE_BLACK : VOXEL_TYPE_WHITE;
                voxel->type = random_voxel_type();
            }
        }
    }
}

void add_cube_vertices(Vec3r *vertices, Vec3r *colours, size_t *index, uint8_t x, uint8_t y, uint8_t z, Vec4r colour) {
    Vec3r cubeVertices[] = CUBE_VERTS;
    Vec3r offset = {x, y, z};

    for (size_t i = 0; i < CUBE_VERTEX_COUNT; i++) {
        cubeVertices[i] = vec3r_scalar(&cubeVertices[i], CUBE_SIZE_SCALING);
        vertices[*index] = vec3r_add(&cubeVertices[i], &offset);
        colours[*index] = (Vec3r){colour.x, colour.y, colour.z};
        (*index)++;
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
        cubeIndices[i].x += offset * CUBE_VERTEX_COUNT;
        cubeIndices[i].y += offset * CUBE_VERTEX_COUNT;
        cubeIndices[i].z += offset * CUBE_VERTEX_COUNT;

        indices[(*index)++] = cubeIndices[i];
    }
}

// use temp scratch buffer to generate mesh data
void chunk_make_mesh(Chunk *chunk) {
    size_t vertexIndex = 0;
    size_t indexIndex = 0;

    for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
        for (uint8_t y = 0; y < CHUNK_SIZE; y++) {
            for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
                // chunk->voxels[]
                Voxel *voxel = chunk_get_voxel(chunk, x, y, z);
                printf("%d, %d, %d : %lu\n", x, y, z, chunk_get_index(x,y,z));   
                Vec4r colour = voxel_color(voxel->type);
                add_cube_vertices((Vec3r *) vertexList, (Vec3r *) colourList, &vertexIndex, x, y, z, colour);
                add_cube_indices((Vec3i *) indexList, &indexIndex, x, y, z);
            }
        }
    }

    printf("vertex count: %d\n", vertexIndex);
    printf("index count: %d\n", indexIndex);

    // vx, vy, vz, cx, cy, cz, nx, ny, nz,
    chunk->vaoID = create_vertex_array();
    bind_vertex_array(chunk->vaoID);

    chunk->vboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertexList, VERTEX_COUNT);
    set_vertex_attrib_pointer(0, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(0);

    GLuint cboID = create_float_buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, colourList, VERTEX_COUNT);
    set_vertex_attrib_pointer(1, 3, GL_FLOAT, 0, NULL);
    enable_vertex_attrib_pointer(1);

    chunk->eboID = create_int_buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indexList, INDEX_COUNT);

    bind_vertex_array(0);
}

void add_face(Vec3r* chunkPos, Vec3r* centerPos, Voxel* center) {
    Vec4r color = voxel_color(center->type);
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
    // TODO: fill in vertices, color, normals : vert index winding order (Counter Clockwise)
    Vec4r color = voxel_color(center->type);
    if (!top) {

    }
    if (!bottom) {
        
    }
    if (!left) {
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
    if (!right) {
        
    }
    if (!front) {
        Vec3r verts[] = FRONT_FACE_VERTS;
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
    if (!back) {
        Vec3r verts[] = BACK_FACE_VERTS;
        unsigned int idx[] = BACK_FACE_INDS;

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
}

void chunk_make_mesh2(Chunk *chunk) {
    for (int8_t x = 0; x < CHUNK_SIZE; x++) {
        for (int8_t y = 0; y < CHUNK_SIZE; y++) {
            for (int8_t z = 0; z < CHUNK_SIZE; z++) {
                Voxel *voxel = chunk_get_voxel(chunk, x, y, z);
                Voxel *top = chunk_get_voxel(chunk, x, y+1, z);
                Voxel *bottom = chunk_get_voxel(chunk, x-1, y, z);
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

// float verts[] = {
//     // front
//     -1,  1, -1, // 0
//      1,  1, -1, // 1
//     -1, -1, -1, // 2
//      1, -1, -1, // 3
//     // back
//     -1,  1, 1, // 4
//      1,  1, 1, // 5
//     -1, -1, 1, // 6
//      1, -1, 1, // 7
//     // left
//      1,  1, -1, // 8
//      1,  1,  1, // 9
//      1, -1, -1, // 10
//      1, -1,  1, // 11
//     // right
//     -1,  1, -1, // 12
//     -1,  1,  1, // 13
//     -1, -1, -1, // 14
//     -1, -1,  1, // 15
//     // top
//     -1, 1,  1, // 16
//      1, 1,  1, // 17
//     -1, 1, -1, // 18
//      1, 1, -1, // 19
//     // bottom
//     -1, -1,  1, // 20
//      1, -1,  1, // 21
//     -1, -1, -1, // 22
//      1, -1, -1, // 23
// };

// float colors[] = {
//     255,0,0,
//     0,255,0,
//     0,0,255,
//     85,0,85,
//     255,0,0,
//     0,255,0,
//     0,0,255,
//     85,0,85,
//     255,0,0,
//     0,255,0,
//     0,0,255,
//     85,0,85,
//     255,0,0,
//     0,255,0,
//     0,0,255,
//     85,0,85,
//     255,0,0,
//     0,255,0,
//     0,0,255,
//     85,0,85,
//     255,0,0,
//     0,255,0,
//     0,0,255,
//     85,0,85,
// };

// unsigned int inds[] = {
//     0,1,3,
//     0,3,2,
//     5,4,7,
//     7,4,6,
//     8,9,11,
//     8,11,10,
//     12,14,15,
//     12,15,13,
//     16,19,18,
//     16,17,19,
//     20,23,21,
//     20,22,23,
// };