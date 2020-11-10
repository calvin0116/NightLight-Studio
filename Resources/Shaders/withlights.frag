#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;
in mat4 viewtransform;

out vec4 fragColor;

struct DirLight {
    vec4 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
    vec4 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float attenuation;
};

struct SpotLight {
    vec4 position;
    vec4 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float cutOff;
    float outerCutOff;
    float attenuation;
};

// Fragment material
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

// Current maximum permitted lights per type
#define MAX_LIGHTS 30

layout (std140) uniform LightCalcBlock
{
    DirLight dLights[MAX_LIGHTS];
    PointLight pLights[MAX_LIGHTS];
    SpotLight sLights[MAX_LIGHTS];

    // Number of lights currently in scene
    int dLights_Num;
    int pLights_Num;
    int sLights_Num;

    vec4 viewPos;
};

// Function declarations
// vec3 CalcDLight(DirLight light, vec3 Normal, vec3 viewDir);
// vec3 CalcPLight(PointLight light, vec3 Normal, vec3 fragPos, vec3 viewDir);
// vec3 CalcSLight(SpotLight light, vec3 Normal, vec3 fragPos, vec3 viewDir);

void main(void)
{
    // properties
    // vec3 norm = normalize(normal);
    // vec3 viewDir = normalize(viewPos.xyz - fragPos);
    vec3 result = vec3(0.f,0.f,0.f);
    
    // // Calculation for all directional lights
    // for(int i = 0; i < dLights_Num; i++)
    //     result += CalcDLight(dLights[i], norm, viewDir);
    // // Calculation for all point lights
    // for(int j = 0; j < pLights_Num; j++)
    //     result += CalcPLight(pLights[j], norm, fragPos, viewDir);    
    // // Calculation for all spot lights
    // for(int k = 0; k < pLights_Num; k++)
    // result += CalcSLight(sLights[k], norm, fragPos, viewDir);    
    
    fragColor = vec4(result, 1.0);
}

// // calculates the color when using a directional light.
// vec3 CalcDLight(DirLight light, vec3 Normal, vec3 viewDir)
// {
//     //vec3 lightDir = normalize(-vec3(viewtransform * vec4(light.direction, 1.0f)));
//     vec3 lightDir = normalize(-light.direction.xyz);
//     // diffuse shading
//     float diff = max(dot(Normal, lightDir), 0.0f);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, Normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
//     // combine results
//     vec3 retAmbient = light.ambient.xyz * ambient;
//     vec3 retDiffuse = light.diffuse.xyz * diff * diffuse;
//     vec3 retSpecular = light.specular.xyz * spec * specular;
//     return (retAmbient + retDiffuse + retSpecular);
// }

// // calculates the color when using a point light.
// vec3 CalcPLight(PointLight light, vec3 Normal, vec3 fragPos, vec3 viewDir)
// {
//     //vec3 lightDir = normalize(vec3(viewtransform * vec4(light.position, 1.0f)) - fragPos);
//     vec3 lightDir = normalize(light.position.xyz - fragPos);
//     // diffuse shading
//     float diff = max(dot(Normal, lightDir), 0.0f);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, Normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
//     // attenuation
//     //float distance = length(vec3(viewtransform * vec4(light.position, 1.0f)) - fragPos);
//     float distance = length(light.position.xyz - fragPos);
//     float attenuation = 1.0f / (1.0f + light.attenuation * (distance * distance));
//     // combine results
//     vec3 retAmbient = light.ambient.xyz * ambient;
//     vec3 retDiffuse = light.diffuse.xyz * diff * diffuse;
//     vec3 retSpecular = light.specular.xyz * spec * specular;
//     retAmbient *= attenuation;
//     retDiffuse *= attenuation;
//     retSpecular *= attenuation;
//     return (retAmbient + retDiffuse + retSpecular);
// }

// // calculates the color when using a spot light.
// vec3 CalcSLight(SpotLight light, vec3 Normal, vec3 fragPos, vec3 viewDir)
// {
//     //vec3 lightDir = normalize(vec3(viewtransform * vec4(light.position, 1.0f)) - fragPos);
//     vec3 lightDir = normalize(light.position.xyz - fragPos);
//     // diffuse shading
//     float diff = max(dot(Normal, lightDir), 0.0f);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, Normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
//     // attenuation
//     //float distance = length(vec3(viewtransform * vec4(light.position, 1.0f)) - fragPos);
//     float distance = length(light.position.xyz - fragPos);
//     float attenuation = 1.0f / (1.0f + light.attenuation * (distance * distance));    
//     // spotlight intensity
//     //float theta = dot(lightDir, normalize(-vec3(viewtransform * vec4(light.direction, 1.0f)))); 
//     float theta = dot(lightDir, normalize(-light.direction.xyz)); 
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
//     // combine results
//     vec3 retAmbient = light.ambient.xyz * ambient;
//     vec3 retDiffuse = light.diffuse.xyz * diff * diffuse;
//     vec3 retSpecular = light.specular.xyz * spec * specular;
//     retAmbient *= attenuation * intensity;
//     retDiffuse *= attenuation * intensity;
//     retSpecular *= attenuation * intensity;
//     return (retAmbient + retDiffuse + retSpecular);
// }