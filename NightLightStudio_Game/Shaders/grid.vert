#version 330 core
layout (location = 0) in vec3 pos;

uniform vec4 rgba;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

out vec4 color;

void main(void) {
    gl_Position = projection * view * vec4(pos, 1.0f);
    color = rgba;
}