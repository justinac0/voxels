#version 330 core

layout (location = 0) in vec3 vertexData;
layout (location = 1) in vec3 colorData;
layout (location = 2) in vec2 texData;

uniform mat4 m_projection;
uniform mat4 m_view;
uniform mat4 m_transform;

out vec3 fragColor;
out vec3 fragPosition;

void main() {
    gl_Position = m_projection * m_view * m_transform * vec4(vertexData, 1);
    fragPosition = vec3(m_transform * vec4(vertexData, 1.0));
    fragColor = colorData;
}