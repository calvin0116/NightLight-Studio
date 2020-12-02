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
    // 48
    struct DirLight {
        glm::vec4 _direction; // 0 - 15
        glm::vec4 _diffuse;   // 16 - 31

        float _intensity;     // 32 - 35

        float _dummyPadding0 = 0.f; // Alignment is rounded up to the base alignment of vec4 36 - 39
        float _dummyPadding1 = 0.f; // Alignment is rounded up to the base alignment of vec4 40 - 43
        float _dummyPadding2 = 0.f; // Alignment is rounded up to the base alignment of vec4 44 - 47

        DirLight(const glm::vec4& Direction = glm::vec4(1.f,0.f,0.f, 0.f), const glm::vec4& Diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                 const float& Intensity = 100.f)
            : _direction{ Direction },
            _diffuse{ Diffuse },
            _intensity{ Intensity }
        {}

        ~DirLight() {}
    };

    // Note, padding is required to fit GLSL alignment
    // 48
    // next PointLight object will start from base alignment of 48
    struct PointLight {
        glm::vec4 position; // 0 - 15

        glm::vec4 _diffuse; // 16 - 31

        float _radius;      // 32 - 35
        float _intensity;   // 36 - 39

        float _dummyPadding0 = 0.f; // Alignment is rounded up to the base alignment of vec4 40 - 43
        float _dummyPadding1 = 0.f; // Alignment is rounded up to the base alignment of vec4 44 - 47

        PointLight(const glm::vec4& Diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f), const float& Intensity = 100.f, const float& Radius = 10.f)
            : position{ 0.f,0.f,0.f, 1.f },
            _diffuse{ Diffuse },
            _radius{ Radius },
            _intensity{ Intensity }
        {}

        ~PointLight() {}
    };

    // Note, padding is required to fit GLSL alignment
    // 64
    // next PointLight object will start from base alignment of 64
    struct SpotLight {
        glm::vec4 position; // 0 - 15
        glm::vec4 _direction; // 16 - 31

        glm::vec4 _diffuse; // 32 - 47

        float _cutOff; // 48 - 51

        float _outerCutOff; // 52 - 55

        float _intensity; // 56 - 59

        float _dummyPadding0 = 0.f; // Alignment is rounded up to the base alignment of vec4 60 - 64

        SpotLight(const glm::vec4& Direction = glm::vec4(1.f, 0.f, 0.f, 0.f), const glm::vec4& Diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f),
                  const float& Intensity = 2.f, const float& CutOff = 10.f, const float& OuterCutOff = 15.f)
            : position{ 0.f,0.f,0.f, 1.f },
            _direction{ Direction },
            _diffuse{ Diffuse },
            _intensity{ Intensity },
            _cutOff{ CutOff },
            _outerCutOff{ OuterCutOff }
        {}

        ~SpotLight() {}
    };
}