#version 330 core
layout (location = 0) out vec4 gPositionAlpha;
layout (location = 1) out vec4 gNormalMetallic;
layout (location = 2) out vec4 gAlbedoRoughness;
layout (location = 3) out vec4 gAmbientOcclusion;

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;
in mat3 TBN;

// PBR Materials
uniform sampler2D AlbedoTex;
uniform sampler2D MetallicTex;
uniform sampler2D RoughnessTex;
uniform sampler2D AOTex; // Might need extra uniform to determine if AO is available
uniform sampler2D NormalTex;

uniform float Alpha;

uniform float RoughnessControl;
uniform float MetallicControl;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPositionAlpha.xyz = fragPos;
    // store alpha in gPositionAlpha's alpha component
    gPositionAlpha.a = Alpha;
    // also store the per-fragment normals into the gbuffer
    gNormalMetallic.xyz = (texture(NormalTex, texCoords).rgb * 2.f - 1.f) * TBN;
    // store metallic intensity in gAlbedoRoughness's alpha component
    gNormalMetallic.a = texture(MetallicTex, texCoords).r * max(MetallicControl, 0.001f);
    // and the diffuse per-fragment color
    gAlbedoRoughness.rgb = texture(AlbedoTex, texCoords).rgb;
    // store roughness intensity in gAlbedoRoughness's alpha component
    gAlbedoRoughness.a = max(texture(RoughnessTex, texCoords).r, 0.001f) * max(RoughnessControl, 0.001f);
    // store ambient occlusion (no texture = 1.f)
    gAmbientOcclusion = vec4(1.f, 1.f, 1.f, 1.f);
}