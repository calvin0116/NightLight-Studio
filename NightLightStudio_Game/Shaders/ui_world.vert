#version 330 core

layout (location = 0) in vec3 pos;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

uniform vec3 ui_pos;
uniform vec3 ui_size;
uniform vec3 camera_right;
uniform vec3 camera_up;

uniform uint maxColumn;
uniform uint maxRow;
uniform uint currentFrame;

out vec2 texCoords;

void main(void) 
{
	vec3 vertexWorldPos = ui_pos
						+ camera_right * pos.x * ui_size.x
						+ camera_up * pos.y * ui_size.y;
    gl_Position = projection * view * vec4(vertexWorldPos, 1.0f);
	
		
	vec2 currTexCoord = pos.xy + vec2(0.5f,0.5f);
	currTexCoord.y =1.0f - currTexCoord.y;
	
	float offsetX;
	float offsetY;
	
	uint row = currentFrame / maxColumn;
	uint column = currentFrame % maxColumn;
	
	offsetX = column * 1.0f/maxColumn;
	offsetY = row * 1.0f/maxRow;
	
	texCoords = vec2(currTexCoord.x/maxColumn + offsetX,currTexCoord.y/maxRow + offsetY);
}