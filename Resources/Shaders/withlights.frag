#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float attenuation;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float attenuation;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
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
    // Number of lights currently in scene
    int dLights_Num;
    int pLights_Num;
    int sLights_Num;

    vec3 viewPos;

    DirLight dLights[MAX_LIGHTS];
    PointLight pLights[MAX_LIGHTS];
    SpotLight sLights[MAX_LIGHTS];
};

// Function declarations
vec3 CalcDLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void)
{
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.f,0.f,0.f);
    
    // Calculation for all directional lights
    for(int i = 0; i < dLights_Num; i++)
        result += CalcDLight(dLights[i], norm, viewDir);
    // Calculation for all point lights
    for(int j = 0; j < pLights_Num; j++)
        result += CalcPLight(pLights[j], norm, fragPos, viewDir);    
    // Calculation for all spot lights
    for(int k = 0; k < pLights_Num; k++)
    result += CalcSLight(sLights[k], norm, fragPos, viewDir);    
    
    fragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 retAmbient = light.ambient * ambient;
    vec3 retDiffuse = light.diffuse * diff * diffuse;
    vec3 retSpecular = light.specular * spec * specular;
    return (retAmbient + retDiffuse + retSpecular);
}

// calculates the color when using a point light.
vec3 CalcPLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.attenuation * (distance * distance));    
    // combine results
    vec3 retAmbient = light.ambient * ambient;
    vec3 retDiffuse = light.diffuse * diff * diffuse;
    vec3 retSpecular = light.specular * spec * specular;
    retAmbient *= attenuation;
    retDiffuse *= attenuation;
    retSpecular *= attenuation;
    return (retAmbient + retDiffuse + retSpecular);
}

// calculates the color when using a spot light.
vec3 CalcSLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.attenuation * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 retAmbient = light.ambient * ambient;
    vec3 retDiffuse = light.diffuse * diff * diffuse;
    vec3 retSpecular = light.specular * spec * specular;
    retAmbient *= attenuation * intensity;
    retDiffuse *= attenuation * intensity;
    retSpecular *= attenuation * intensity;
    return (retAmbient + retDiffuse + retSpecular);
}