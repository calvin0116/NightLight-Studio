#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in mat4 model;

uniform mat4 ortho_proj;

out vec2 texCoords;

void main(void) {
    gl_Position = ortho_proj * model * vec4(pos, 1.0f);
    texCoords = uv;
}