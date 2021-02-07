#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

// Emissive color
uniform vec3 emissive;
uniform float Alpha;

uniform float Gamma;

void main(void)
{
    // Tonemapping
    //vec3 resultColor = emissive / (emissive + vec3(1.0f));

    // Gamma correction
    // 2.2f is a default gamma value that is the average gamma for most displays
    // Might replace with uniform gamma value
    //resultColor = pow(resultColor, vec3(1.0f/Gamma));

    fragColor = vec4(emissive, Alpha);
}