#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

GLuint create_shader_program(char* vertexFilePath, char* fragmentFilePath);

#endif // SHADER_H