#version 330 core
in vec4 blendColor;
in vec2 texCoords;
out vec4 fragmtColor;

void main(void)
{
    fragmtColor = blendColor;
    texCoords;
}