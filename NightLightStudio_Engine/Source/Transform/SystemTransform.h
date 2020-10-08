#pragma once

#include "../Core/MySystem.h"
#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "..\Component\ComponentTransform.h"

#include "../glm/glm.hpp"   // glm::mat4
#include "../glm/ext/matrix_transform.hpp"


class ENGINE_API SystemTransform : public MySystem, public Singleton<SystemTransform>
{
    SystemTransform() = default;
    ~SystemTransform() = default;

    friend Singleton<SystemTransform>;

public:
    // System functions
    void OnFirstStart() override {};

    void EarlyLoad() override {};
    void Load() override {};
    void LateLoad() override {};

    void EarlyInit() override {};
    void Init() override {};
    void LateInit() override {};

    bool FixedUpdate() override { return true; };
    bool Update() override;
    bool LateUpdate() override { return true; };

    void Exit() override {};
    void Free() override {};
    void Unload() override {};
};

//Probably wont need this
//GLOBAL pointer to an instance of transform system
//== Good to have for every system so that you dont need to always get instance
static SystemTransform* SYS_TRANSFORM = SystemTransform::GetInstance();