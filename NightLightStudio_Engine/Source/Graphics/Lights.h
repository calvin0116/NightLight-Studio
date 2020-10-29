#include "../glm/glm.hpp"   // glm::mat4

#pragma once

namespace NS_GRAPHICS
{
    // Different light types declared and defined here
    enum class Lights
    {
        DIRECTIONAL = 0,
        POINT,
        SPOT,
        INVALID_TYPE // In case of error returned
    };

    struct DirLight {
        glm::vec3 _direction;

        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;

        DirLight(const glm::vec3& Direction, const glm::vec3& Ambient,
                 const glm::vec3& Diffuse, const glm::vec3& Specular)
            : _direction{ Direction },
            _ambient{ Ambient },
            _diffuse{ Diffuse },
            _specular{ Specular }
        {}

        ~DirLight() {}
    };

    struct PointLight {
        //glm::vec3 position;

        // Loss of light intensity over distance, the greater the distance, the lower the intensity
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float _attenuation;

        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;

        PointLight(const float& Attenuation, const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular)
            : //position{ Position },
            _attenuation{ Attenuation },
            _ambient{ Ambient },
            _diffuse{ Diffuse },
            _specular{ Specular }
        {}

        ~PointLight() {}
    };

    struct SpotLight {
        //glm::vec3 position;
        glm::vec3 _direction;
        float _cutOff;
        float _outerCutOff;

        // Loss of light intensity over distance
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float _attenuation;

        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;

        SpotLight(const glm::vec3& Direction, const float& Attenuation,
                  const float& CutOff, const float& OuterCutOff,
                  const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular)
            : //position{ Position },
            _direction{ Direction },
            _attenuation{ Attenuation },
            _cutOff{ CutOff },
            _outerCutOff{ OuterCutOff },
            _ambient{ Ambient },
            _diffuse{ Diffuse },
            _specular{ Specular }
        {}

        ~SpotLight() {}
    };
}