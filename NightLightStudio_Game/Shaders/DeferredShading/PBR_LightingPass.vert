#version 330 core
layout (location = 0) in vec3 screenPos;
layout (location = 1) in vec2 screenTexCoords;

out vec2 texCoords;

void main()
{
    // This is the provided screen "texture" which we will reference for lighting calculation
    texCoords = screenTexCoords;
    gl_Position = vec4(screenPos, 1.0);
}