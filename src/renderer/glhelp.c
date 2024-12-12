#include "glhelp.h"

GLuint create_vertex_array() {
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);

    return vaoID;
}

void bind_vertex_array(GLuint id) {
    glBindVertexArray(id);
}

GLuint create_float_buffer(GLenum type, GLenum usage, float *data, uint32_t count) {
    GLuint bID;
    glGenBuffers(1, &bID);
    glBindBuffer(type, bID);
    glBufferData(type, sizeof(float) * count, data, usage);

    return bID;
}

GLuint create_short_buffer(GLenum type, GLenum usage, unsigned short *data, uint32_t count) {
    GLuint bID;
    glGenBuffers(1, &bID);
    glBindBuffer(type, bID);
    glBufferData(type, sizeof(unsigned short) * count, data, usage);

    return bID;
}


GLuint create_int_buffer(GLenum type, GLenum usage, unsigned int *data, uint32_t count) {
    GLuint bID;
    glGenBuffers(1, &bID);
    glBindBuffer(type, bID);
    glBufferData(type, sizeof(unsigned int) * count, data, usage);

    return bID;
}

void set_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
}

void enable_vertex_attrib_pointer(GLuint index) {
    glEnableVertexAttribArray(index);
}
