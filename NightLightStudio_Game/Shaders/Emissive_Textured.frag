#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

// Emissive color
uniform vec3 emissive;
uniform float emissiveIntensity;
uniform sampler2D AlbedoTex;

uniform float Alpha;

void main(void)
{
    // Tonemapping
    //vec3 resultColor = emissive / (emissive + vec3(1.0f));

    fragColor = vec4((texture(AlbedoTex, texCoords).rgb * (1.f/emissiveIntensity)) * emissive, Alpha);
}