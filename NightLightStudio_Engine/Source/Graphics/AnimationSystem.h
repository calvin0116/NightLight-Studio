#pragma once
#include "../Window/WndUtils.h"
#include "../Window/WndSystem.h"

#include "../Core/MySystem.h"
#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "../Component/ComponentManager.h"

#include "ModelManager.h"
#include "AnimationController.h"

#include "../glm/glm.hpp"   // glm::mat4

namespace NS_GRAPHICS
{
    class AnimationSystem
    {
        AnimationSystem();
        ~AnimationSystem();

    public:

        static AnimationSystem& GetInstance()
        {
            static AnimationSystem instance;
            return instance;
        }

        void Update();
        void Free();
        void Init();
        void Exit();

        std::vector<AnimationController*> _animControllers;
        std::vector<bool> _usedStatus;

        unsigned AddAnimController();
        bool RemoveAnimControllerByID(const int& index);

        size_t GetFreeIndex();
    };
}

