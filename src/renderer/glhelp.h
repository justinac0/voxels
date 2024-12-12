#ifndef GLHELP_H
#define GLHELP_H

#include <glad/glad.h>

typedef struct {
    GLfloat position[3];
    GLfloat normal[3];
    GLubyte color[4];
} Vertex;

GLuint create_vertex_array();
void bind_vertex_array(GLuint id);
GLuint create_float_buffer(GLenum type, GLenum usage, float *data, uint32_t count);
GLuint create_short_buffer(GLenum type, GLenum usage, short *data, uint32_t count);
GLuint create_int_buffer(GLenum type, GLenum usage, int *data, uint32_t count);
void set_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
void enable_vertex_attrib_pointer(GLuint index);

#endif // GLHELP_H