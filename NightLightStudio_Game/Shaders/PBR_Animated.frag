#version 330 core
layout (location = 0) out vec4 gPositionAlpha;
layout (location = 1) out vec4 gNormalMetallic;
layout (location = 2) out vec4 gAlbedoRoughness;
layout (location = 3) out vec4 gAmbientOcclusion;

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

uniform vec3 Albedo;
uniform float Metallic;
uniform float Roughness;
uniform float Alpha;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPositionAlpha.xyz = fragPos;
    // store alpha in gPositionAlpha's alpha component
    gPositionAlpha.a = Alpha;
    // also store the per-fragment normals into the gbuffer
    gNormalMetallic.xyz = normal;
    // store metallic intensity in gAlbedoRoughness's alpha component
    gNormalMetallic.a = Metallic;
    // and the diffuse per-fragment color
    gAlbedoRoughness.rgb = Albedo;
    // store roughness intensity in gAlbedoRoughness's alpha component
    gAlbedoRoughness.a = Roughness;
    // store ambient occlusion (no texture = 1.f)
    gAmbientOcclusion = vec4(1.f);
}