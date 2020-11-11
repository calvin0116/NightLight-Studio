#version 330 core
//Read in position and uv
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in mat4 model;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

//For fragment shader
out vec2 fragmentTexture;

//This is for single frame texture. E.g like floor tiles 
void main()
{
	gl_Position = vec4(pos,1.0f);
    fragmentTexture = vec2(uv.x,uv.y);
}
