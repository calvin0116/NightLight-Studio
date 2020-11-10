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
    // 64
    struct DirLight {
        glm::vec4 _direction;
        
        glm::vec4 _ambient;

        glm::vec4 _diffuse;

        glm::vec4 _specular;

        DirLight(const glm::vec4& Direction = glm::vec4(1.f,0.f,0.f, 0.f), const glm::vec4& Ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.f),
                 const glm::vec4& Diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), const glm::vec4& Specular = glm::vec4(1.f, 1.f, 1.f, 1.f))
            : _direction{ Direction },
            _ambient{ Ambient },
            _diffuse{ Diffuse },
            _specular{ Specular }
        {}

        ~DirLight() {}
    };

    // Note, padding is required to fit GLSL alignment
    // 80
    // next PointLight object will start from base alignment of 80
    struct PointLight {
        glm::vec4 position; // 0 - 15

        glm::vec4 _ambient; // 16 - 31

        glm::vec4 _diffuse; // 32 - 47

        glm::vec4 _specular; // 48 - 63

        // Loss of light intensity over distance, the greater the distance, the lower the intensity
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float _attenuation; // 64 - 67
        float _dummyPadding0 = 0.f; // Alignment is rounded up to the base alignment of vec4
        float _dummyPadding1 = 0.f; // Alignment is rounded up to the base alignment of vec4
        float _dummyPadding2 = 0.f; // Alignment is rounded up to the base alignment of vec4

        PointLight(const float& Attenuation = 2.f, const glm::vec4& Ambient = glm::vec4(1.f, 1.f, 1.f, 1.f),
                  const glm::vec4& Diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f), const glm::vec4& Specular = glm::vec4(1.f, 1.f, 1.f, 1.f))
            : position{ 0.f,0.f,0.f, 1.f },
            _attenuation{ Attenuation },
            _ambient{ Ambient },
            _diffuse{ Diffuse },
            _specular{ Specular }
        {}

        ~PointLight() {}
    };

    // Note, padding is required to fit GLSL alignment
    // 96
    struct SpotLight {
        glm::vec4 position; // 0 - 15

        glm::vec4 _direction; // 16 - 31

        glm::vec4 _ambient; // 32 - 47

        glm::vec4 _diffuse; // 48 - 63

        glm::vec4 _specular; // 64 - 79

        float _cutOff; // 80 - 83

        float _outerCutOff; // 84 - 87

        // Loss of light intensity over distance
        // An attenuation value of 0.2 means that 80% of the light intensity has been lost, and only 20% of the intensity remains
        float _attenuation; // 88 - 91
        float _dummyPadding0 = 0.f; // Alignment is rounded up to the base alignment of vec4


        SpotLight(const glm::vec4& Direction = glm::vec4(1.f, 0.f, 0.f, 0.f), const float& Attenuation = 2.f,
                  const float& CutOff = 10.f, const float& OuterCutOff = 15.f,
                  const glm::vec4& Ambient = glm::vec4(1.f, 1.f, 1.f, 1.f), const glm::vec4& Diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f),
                  const glm::vec4& Specular = glm::vec4(1.f, 1.f, 1.f, 1.f))
            : position{ 0.f,0.f,0.f, 1.f },
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