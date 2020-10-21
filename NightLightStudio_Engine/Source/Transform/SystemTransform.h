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
    void Load() override {};
    void Init() override {};

    void GameInit() override {};

    void FixedUpdate() override { };
    void Update() override;
    //void LateUpdate() override {};

    void GameExit() override {};
    void Free() override {};
    //void Unload() override {};
};

//Probably wont need this
//GLOBAL pointer to an instance of transform system
//== Good to have for every system so that you dont need to always get instance
static SystemTransform* SYS_TRANSFORM = SystemTransform::GetInstance();