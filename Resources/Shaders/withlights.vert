#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;
layout (location = 3) in mat4 model;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;
out mat4 viewtransform;

void main(void) {
    fragPos = vec3(view * model * vec4(pos, 1.0f));
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    normal = vec3(transpose(inverse(view * model)) * vec4(norm, 0.f));
    texCoords = uv;
    viewtransform = view;
}