#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particle and size of the square
layout(location = 2) in vec4 colour; // Color of particle
layout(location = 3) in uint frame; // Color of particle

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

uniform vec3 camera_right;
uniform vec3 camera_up;

uniform uint maxColumn;
uniform uint maxRow;

out vec2 texCoords;
out vec4 particleColour;

void main(void) {
	vec3 particlePos = xyzs.xyz;
	float particleSize = xyzs.w;
	
	vec3 vertexWorldPos = particlePos
						+ camera_right * squareVertices.x * particleSize
						+ camera_up * squareVertices.y * particleSize;
	
	gl_Position = projection * view * vec4(vertexWorldPos, 1.0f);
	
	vec2 currTexCoord = squareVertices.xy + vec2(0.5f,0.5f);
	currTexCoord.y =1.0f - currTexCoord.y;
	
	uint row = frame / maxColumn;
	uint column = frame % maxColumn;
	
	float offsetX = column * 1.0f/maxColumn;
	float offsetY = row * 1.0f/maxRow;
	
	texCoords = vec2(currTexCoord.x/maxColumn + offsetX,currTexCoord.y/maxRow + offsetY);
	
	particleColour = colour;
}