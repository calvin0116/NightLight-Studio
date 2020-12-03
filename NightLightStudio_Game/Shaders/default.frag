#version 330 core
in vec4 blendColor;
out vec4 fragmtColor;

void main(void)
{
    fragmtColor = blendColor;
}