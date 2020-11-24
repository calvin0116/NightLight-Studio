#version 330 core

const int MAX_JOINTS = 64;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;
layout (location = 3) in ivec4 boneID;
layout (location = 4) in vec4 weights;
layout (location = 5) in mat4 model;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

uniform mat4 jointsMat[MAX_JOINTS];

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;

void main(void) {
    fragPos = vec3(model * vec4(pos, 1.0f));
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    normal = vec3(transpose(inverse(model)) * vec4(norm, 0.f));
    texCoords = uv;
}