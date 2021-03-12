#version 330 core

const int MAX_JOINTS = 64;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 tangent;
layout (location = 4) in ivec4 boneID;
layout (location = 5) in vec4 weights;
layout (location = 6) in mat4 model;

uniform mat4 jointsMat[MAX_JOINTS];

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;
void main(void) {

	mat4 boneTrans = mat4(0.0f);
	for(int i = 0; i < 4; i++)
	{
		boneTrans += jointsMat[boneID[i]] * weights[i];
	}

	vec4 newPos = boneTrans * vec4(pos, 1.0f);	
    fragPos = vec3(model * newPos);
    gl_Position = projection * view * model * newPos;

    mat4 normalMatrix = transpose(inverse(model * boneTrans));
    normal = vec3(normalMatrix * vec4(norm, 0.f));

    texCoords = uv;
}