//#pragma once
//#include "../Window/WndUtils.h"
//#include "../Window/WndSystem.h"
//#include "GraphicsSystem.h"
//
//#include "../Core/MySystem.h"
//#include "../IO/Json/Config.h"
//#include "../../framework.h"
//#include "../Component/ComponentManager.h"
//
//#include "ModelManager.h"
//#include "AnimationController.h"
//
//#include "../glm/glm.hpp"   // glm::mat4
//
//
//namespace NS_GRAPHICS
//{
//    class AnimationSystem : public MySystem, public Singleton<AnimationSystem>
//    {
//        AnimationSystem();
//
//        virtual ~AnimationSystem();
//
//        friend Singleton<AnimationSystem>;
//
//    public:
//
//        void Update();
//        void Free();
//        void Init() override;
//        void Exit() override;
//        void Render();
//
//        void AddAnimController();
//
//    private:
//
//        std::map<unsigned, AnimationController*> _animControllers;
//        unsigned _animIndex;
//    };
//
//    //GLOBAL pointer to an instance of graphic system
//    //== Good to have for every system so that you dont need to always get instance
//    static AnimationSystem* SYS_ANIM = AnimationSystem::GetInstance();
//}
