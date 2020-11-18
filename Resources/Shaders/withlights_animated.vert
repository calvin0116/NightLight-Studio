#version 330 core

const int MAX_JOINTS = 50;

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
	vec4 currPose = vec4(0);
	vec4 currNormal = vec4(0);

	for(int i = 0; i < 4; i++)
	{
		//Position
		mat4 transforms = jointsMat[boneID[i]];
		vec4 posePos = transforms * vec4(pos, 1.0f)
		currPose += posePos;
		
		//Normals
	}
    fragPos = vec3(model * vec4(pos, 1.0f));
    gl_Position = projection * view * model * currPose;
    normal = vec3(transpose(inverse(model)) * vec4(norm, 0.f));
    texCoords = uv;
}