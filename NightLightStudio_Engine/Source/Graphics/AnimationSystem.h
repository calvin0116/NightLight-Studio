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

        unsigned AddAnimController();

        std::map<unsigned, AnimationController*> _animControllers;
        unsigned _animIndex = 0;
    };
}
