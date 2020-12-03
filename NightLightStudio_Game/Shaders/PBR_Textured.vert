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
//out mat3 TBN;

void main(void) {
    fragPos = vec3(model * vec4(pos, 1.0f));
    gl_Position = projection * view * model * vec4(pos, 1.0f);

    mat4 normalMatrix = transpose(inverse(model));
    normal = vec3(normalMatrix * vec4(norm, 0.f));

    //vec3 T = normalize(normalMatrix * aTangent);
    //vec3 N = normalize(normalMatrix * aNormal);
    //T = normalize(T - dot(T, N) * N);
    //vec3 B = cross(N, T);
    //TBN = transpose(mat3(T, B, N));

    texCoords = uv;
}