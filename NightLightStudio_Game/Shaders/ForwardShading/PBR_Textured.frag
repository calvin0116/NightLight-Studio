#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 fragNormal;
in mat3 TBN;

out vec4 fragColor;

struct DirLight {
    vec4 direction;
    
    vec4 diffuse;

    float intensity;
};

struct PointLight {
    vec4 position;

    vec4 diffuse;

    float radius;
    float intensity;
};

struct SpotLight {
    vec4 position;
    vec4 direction;

    vec4 diffuse;

    float cutOff;
    float outerCutOff;
    float intensity;
};

// PBR Materials
uniform sampler2D AlbedoTex;
uniform sampler2D MetallicTex;
uniform sampler2D RoughnessTex;
uniform sampler2D AOTex; // Might need extra uniform to determine if AO is available
uniform sampler2D NormalTex;
uniform float Alpha;

uniform float Gamma;

uniform float RoughnessControl;
uniform float MetallicControl;

// Current maximum permitted lights per type
#define MAX_LIGHTS 512

// defined PI
const float PI = 3.14159265359f;

layout (std140) uniform LightCalcBlock
{
    DirLight dLight[1];
    PointLight pLights[MAX_LIGHTS];
    SpotLight sLights[MAX_LIGHTS];

    // Number of lights currently in scene
    int dLight_Num;
    int pLights_Num;
    int sLights_Num;

    vec4 viewPos;
};

float DistributionGGX(float NdotH, float roughness)
{
    float a = roughness * roughness;
    float aSqred = a * a;
    float denom = (NdotH * NdotH) * (aSqred - 1.0f) + 1.0f;
    denom = PI * denom * denom;
    return aSqred / max(denom, 0.000001f); // MUST avoid division by 0
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    // Base Reflectivity will always be in range of 0.f to 1.f
    // SchlickGGX returns range of BaseReflectivity to 1.f
    // Return value increases as HdotV decreases
    // More reflectivity/increased highlights when surface viewed at larger angles
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5);
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    float ggx1 = NdotV / (NdotV * (1.0f - k) + k);
    float ggx2 = NdotL / (NdotL * (1.0f - k) + k);

    return ggx1 * ggx2;
}

float lengthSqr(vec3 A,vec3 B)
{
    vec3 C = A - B;
    return dot( C, C );
}

void main(void)
{
    // Change the following to TBN space: lightPos, viewPos, fragPos
    vec3 _fragPos = fragPos;
    vec3 _viewPos = viewPos.xyz;

    // In case of textures, calculate properties for each point
    vec3 albedo = texture(AlbedoTex, texCoords).rgb;
    float metallic = texture(MetallicTex, texCoords).r;
    float roughness = max(texture(RoughnessTex, texCoords).r, 0.001f);
    vec3 normal = texture(NormalTex, texCoords).rgb; // for normal map

    roughness *= max(RoughnessControl, 0.00001f);
    metallic *= max(MetallicControl, 0.00001f);
    //float ao = texture(AOTex, texCoords).r;
    float ao = 1.f;

    // properties
    vec3 N = normalize((normal * 2.f - 1.f) * TBN); // Convert normal to tangent space
    //vec3 N = normalize(fragNormal); // required normal vector
    vec3 V = normalize(_viewPos - _fragPos); // required view vector

    // required Base reflectivity
    vec3 F0 = vec3(0.04f);

    // calculate reflectance at normal incidence
    // mix function interpolates between two values, F0 and albedo in our case
    // if dia-electric(metallic = 0), we get F0
    // if metal(metallic = 1), we get albedo

    // for metallic between 0 to 1, we get interpolated values in between the two
    // in this case, if metallic is closer to 1, we get value closer to albedo
    F0 = mix(F0, albedo, metallic);

    // Reflectance equation
    // We accumulate the output luminance in this variable
    vec3 Lo = vec3(0.f);

    // Calculate lights here
    // Calculation for all directional lights
    // light vector is handled differently
    for(int i = 0; i < dLight_Num; i++)
    {
        // Convert light direction/position to tangent space
        // calculate per-light radiance
        vec3 L = normalize(-(dLight[i].direction.xyz)); // light vector
        vec3 H = normalize(V + L); // Halfway-bisecting vector
        vec3 radiance = dLight[i].diffuse.xyz * dLight[i].intensity;

        // Cook-Torrance BRDF
        // epsilon to avoid division by zero
        float NdotV = max(dot(N, V), 0.0001f); 
        float NdotL = max(dot(N, L), 0.0001f);
        float HdotV = max(dot(H, V), 0.0f);
        float NdotH = max(dot(N, H), 0.0f);

        float D = DistributionGGX(NdotH, roughness);
        float G = GeometrySmith(NdotV, NdotL, roughness);
        vec3 F = FresnelSchlick(HdotV, F0); // kS

        vec3 specular = D * G * F;
        specular /= (4.f * NdotV * NdotL);

        // Energy Conservation where diffuse + specular <= 1.f
        // Calculation of diffuse factor
        vec3 kD = vec3(1.f) - F;

        // Multiply kD by inverse metalness
        // since only dia-electric materials have diffuse lighting
        // Linear blend if partially metal
        kD *= 1.f - metallic;

        // angle of light to surface affects specular and diffuse
        // Mix albedo with diffuse, but not specular
        // This is just how reality works
        // Specular component bounces off the surface
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // Calculation for all point lights
    // Should be quite easy due to sample code
    for(int j = 0; j < pLights_Num; j++)
    {
        float distanceSqr = lengthSqr(pLights[j].position.xyz, fragPos);

        if(distanceSqr < pLights[j].radius * pLights[j].radius)
        {
            // calculate per-light radiance
            vec3 L = normalize(pLights[j].position.xyz - fragPos); // light vector
            vec3 H = normalize(V + L); // Halfway-bisecting vector
            float distance = length(pLights[j].position.xyz - fragPos);
            //float attenuation = 1.f / (distance * distance); // inverse squared
            float attenuation = smoothstep(pLights[j].radius, 0.f, distance); // where 100.f is the radius

            vec3 radiance = ((pLights[j].diffuse.rgb * pLights[j].intensity) * attenuation); // diffuse used in place of color

            // Cook-Torrance BRDF
            // epsilon to avoid division by zero
            float NdotV = max(dot(N, V), 0.0001f);
            float NdotL = max(dot(N, L), 0.0001f);
            float HdotV = max(dot(H, V), 0.0f);
            float NdotH = max(dot(N, H), 0.0f);

            //fragColor = vec4(NdotH,0.f,0.f, 1.f); // 1.f should be replaced with uniform later
            //return;

            float D = DistributionGGX(NdotH, roughness);
            float G = GeometrySmith(NdotV, NdotL, roughness);
            vec3 F = FresnelSchlick(HdotV, F0); // kS

            vec3 specular = D * G * F;
            specular /= (4.f * NdotV * NdotL);

            // Energy Conservation where diffuse + specular <= 1.f
            // Calculation of diffuse factor
            vec3 kD = vec3(1.f) - F;

            // Multiply kD by inverse metalness
            // since only dia-electric materials have diffuse lighting
            // Linear blend if partially metal
            kD *= 1.f - metallic;

            // angle of light to surface affects specular and diffuse
            // Mix albedo with diffuse, but not specular
            // This is just how reality works
            // Specular component bounces off the surface
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;
        }
    }

    // Calculation for all spot lights
    // Similar to point light calculation
    for(int k = 0; k < sLights_Num; k++)
    {
        // calculate per-light radiance
        vec3 L = normalize(sLights[k].position.xyz - fragPos); // light vector
        vec3 H = normalize(V + L); // Halfway-bisecting vector
        float distance = length(sLights[k].position.xyz - fragPos);
        //float attenuation = 1.f / (distance * distance); // inverse squared

        float theta = dot(L, normalize(-sLights[k].direction.xyz));

        float attenuation = smoothstep(theta, -radians(sLights[k].cutOff), distance); // where 100.f is the radius

        float epsilon = sLights[k].cutOff - sLights[k].outerCutOff;

        //float intensity = clamp((theta - sLights[k].outerCutOff) / epsilon, 0.f, 1.f);
        float intensity = min(pow((theta - sLights[k].outerCutOff) / epsilon, 6.f), 1.f);

        vec3 radiance = (sLights[k].diffuse.xyz * sLights[k].intensity)
                        * attenuation * intensity; // diffuse used in place of color

        // Cook-Torrance BRDF
        // epsilon to avoid division by zero
        float NdotV = max(dot(N, V), 0.0001f); 
        float NdotL = max(dot(N, L), 0.0001f);
        float HdotV = max(dot(H, V), 0.0f);
        float NdotH = max(dot(N, H), 0.0f);

        float D = DistributionGGX(NdotH, roughness);
        float G = GeometrySmith(NdotV, NdotL, roughness);
        vec3 F = FresnelSchlick(HdotV, F0); // kS

        vec3 specular = D * G * F;
        specular /= (4.f * NdotV * NdotL);

        // Energy Conservation where diffuse + specular <= 1.f
        // Calculation of diffuse factor
        vec3 kD = vec3(1.f) - F;

        // Multiply kD by inverse metalness
        // since only dia-electric materials have diffuse lighting
        // Linear blend if partially metal
        kD *= 1.f - metallic;

        // angle of light to surface affects specular and diffuse
        // Mix albedo with diffuse, but not specular
        // This is just how reality works
        // Specular component bounces off the surface
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }


    // ambient lighting(should be replaced later with environment lighting)
    vec3 finalAmbient = vec3(0.03f) * albedo * ao;

    vec3 resultColor = finalAmbient + Lo;

    // Tonemapping
    resultColor = resultColor / (resultColor + vec3(1.0f));

    // Gamma correction
    // 2.2f is a default gamma value that is the average gamma for most displays
    // Might replace with uniform gamma value
    resultColor = pow(resultColor, vec3(1.f/Gamma));

    fragColor = vec4(resultColor, Alpha);

    // End of PBR calculation
}