#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

// Emissive color
uniform vec3 emissive;
uniform float emissiveIntensity;
uniform vec3 Albedo;

uniform float Alpha;

void main(void)
{
    // Tonemapping
    //vec3 resultColor = emissive / (emissive + vec3(1.0f));

    fragColor = vec4((Albedo * (1.f/emissiveIntensity)) * emissive, Alpha);
}