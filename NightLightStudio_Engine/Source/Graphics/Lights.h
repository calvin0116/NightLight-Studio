#include "../glm/glm.hpp"   // glm::mat4

namespace NS_GRAPHICS
{
    // Different light types declared and defined here

    struct DirLight {
        glm::vec3 direction;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct PointLight {
        glm::vec3 position;

        // Loss of light intensity over distance, the greater the distance, the lower the intensity
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float attenuation;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct SpotLight {
        glm::vec3 position;
        glm::vec3 direction;
        float cutOff;
        float outerCutOff;

        // Loss of light intensity over distance
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float attenuation;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
}