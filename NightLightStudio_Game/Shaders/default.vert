#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 color;
layout (location = 3) in mat4 model;
layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};
out vec4 blendColor;
out vec2 texCoords;
void main(void) {
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    blendColor = vec4(color.xyz, 1.f);
    texCoords = uv;
}