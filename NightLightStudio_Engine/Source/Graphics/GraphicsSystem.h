#pragma once

#include "../Core/MySystem.h"
#include "../IO/Json/Config.h"
#include "../../framework.h"

#include "ShaderSystem.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "ModelManager.h"
#include "LightSystem.h"
#include "DebugManager.h"
#include "CameraSystem.h"

namespace NS_GRAPHICS
{
    class ENGINE_API GraphicsSystem : public MySystem, public Singleton<GraphicsSystem>
    {
        GraphicsSystem();

        virtual ~GraphicsSystem();

        friend Singleton<GraphicsSystem>;

    public:
        /*
            Update the following in the following order:
            > Model Loader Queue
            > Texture Loader Queue
            > Camera Manager
            > Model Instance Manager
            > Debug Instance Manager
            > Light Instance Manager
            > Shader Program Variables
            > Render
        */
        bool Update();

        /*
            Free the following in the following order:
            > Model Queue & Loader
            > Texture Queue & Loader
            > Model Instance Manager
            > Debug Instance Manager
            > Light Instance Manager
            > Cameras & Camera Manager
            > Shader Programs & Manager
        */
        void Free();

        /*
            Initializes the following in the following order:
            > Shader Program Manager
            > Model Loader
            > Texture Loader
            > Model Instance Manager
            > Debug Instance Manager
            > Camera Manager
            > Light Instance Manager
        */
        void Init();

        void Render();

        void ToggleDebugDraw(bool&& set);

    private:

        bool hasInit;
        bool debugDrawing;

        ShaderSystem* shaderManager;
        ModelLoader* modelLoader;
        TextureLoader* textureLoader;
        ModelManager* modelManager;
        LightSystem* lightManager;
        DebugManager* debugManager;
        CameraSystem* cameraManager;
    };

    //GLOBAL pointer to an instance of graphic system
    //== Good to have for every system so that you dont need to always get instance
    static GraphicsSystem* SYS_GRAPHICS = GraphicsSystem::GetInstance();
}