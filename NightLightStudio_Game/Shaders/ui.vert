#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in mat4 model;

uniform mat4 ortho_proj;

uniform uint maxColumn;
uniform uint maxRow;
uniform uint currentFrame;

out vec2 texCoords;

void main(void) {
    gl_Position = ortho_proj * model * vec4(pos, 1.0f);
	
	float offsetX;
	float offsetY;
	
	uint row = currentFrame / maxColumn;
	uint column = currentFrame % maxColumn;
	
	offsetX = column * 1.0f/maxColumn;
	offsetY = row * 1.0f/maxRow;
	
	texCoords = vec2(uv.x/maxColumn + offsetX,uv.y/maxRow + offsetY);
}