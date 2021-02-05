#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particle and size of the square
layout(location = 2) in vec4 colour; // Color of particle

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

uniform vec3 camera_right;
uniform vec3 camera_up;

out vec2 texCoords;
out vec4 particleColour;

void main(void) {
	vec3 particlePos = xyzs.xyz;
	float particleSize = xyzs.w;
	
	vec3 vertexWorldPos = particlePos
						+ camera_right * squareVertices.x * particleSize
						+ camera_up * squareVertices.y * particleSize;
	
    //gl_Position = vec4(squareVertices, 1.0f);
	gl_Position = projection * view * vec4(vertexWorldPos, 1.0f);
    texCoords = squareVertices.xy + vec2(0.5f,0.5f);
	particleColour = colour;
}