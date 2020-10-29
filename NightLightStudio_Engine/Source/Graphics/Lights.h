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

    // Note, padding is required to fit GLSL alignment
    struct DirLight {
        glm::vec3 _direction;
        float _dummyPadding0 = 0.f;

        glm::vec3 _ambient;
        float _dummyPadding1 = 0.f;
        glm::vec3 _diffuse;
        float _dummyPadding2 = 0.f;
        glm::vec3 _specular;
        float _dummyPadding3 = 0.f;

        DirLight(const glm::vec3& Direction = glm::vec3(1.f,0.f,0.f), const glm::vec3& Ambient = glm::vec3(1.f, 1.f, 1.f),
                 const glm::vec3& Diffuse = glm::vec3(1.f, 1.f, 1.f), const glm::vec3& Specular = glm::vec3(1.f, 1.f, 1.f))
            : _direction{ Direction },
            _ambient{ Ambient },
            _diffuse{ Diffuse },
            _specular{ Specular }
        {}

        ~DirLight() {}
    };

    // Note, padding is required to fit GLSL alignment
    struct PointLight {
        glm::vec3 position;
        // No padding required here as float is next

        // Loss of light intensity over distance, the greater the distance, the lower the intensity
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float _attenuation;

        glm::vec3 _ambient;
        float _dummyPadding1 = 0.f;
        glm::vec3 _diffuse;
        float _dummyPadding2 = 0.f;
        glm::vec3 _specular;
        float _dummyPadding3 = 0.f;

        PointLight(const float& Attenuation = 2.f, const glm::vec3& Ambient = glm::vec3(1.f, 1.f, 1.f),
                  const glm::vec3& Diffuse = glm::vec3(1.f, 1.f, 1.f), const glm::vec3& Specular = glm::vec3(1.f, 1.f, 1.f))
            : position{ 0.f,0.f,0.f },
            _attenuation{ Attenuation },
            _ambient{ Ambient },
            _diffuse{ Diffuse },
            _specular{ Specular }
        {}

        ~PointLight() {}
    };

    // Note, padding is required to fit GLSL alignment
    struct SpotLight {
        glm::vec3 position;
        float _dummyPadding0 = 0.f;
        glm::vec3 _direction;
        float _cutOff;
        float _outerCutOff;

        // Loss of light intensity over distance
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float _attenuation;

        glm::vec3 _ambient;
        float _dummyPadding1 = 0.f;
        glm::vec3 _diffuse;
        float _dummyPadding2 = 0.f;
        glm::vec3 _specular;
        float _dummyPadding3 = 0.f;

        SpotLight(const glm::vec3& Direction = glm::vec3(1.f, 0.f, 0.f), const float& Attenuation = 2.f,
                  const float& CutOff = 10.f, const float& OuterCutOff = 15.f,
                  const glm::vec3& Ambient = glm::vec3(1.f, 1.f, 1.f), const glm::vec3& Diffuse = glm::vec3(1.f, 1.f, 1.f), const glm::vec3& Specular = glm::vec3(1.f, 1.f, 1.f))
            : position{ 0.f,0.f,0.f },
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