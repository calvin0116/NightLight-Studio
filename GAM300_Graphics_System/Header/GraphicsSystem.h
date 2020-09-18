#pragma once

#include "ShaderSystem.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "ModelManager.h"
#include "LightSystem.h"
#include "DebugManager.h"
#include "CameraSystem.h"

namespace NS_GRAPHICS
{
    class GraphicsSystem
    {
        GraphicsSystem();

        virtual ~GraphicsSystem();

    public:

        // Unique Singleton instance
        static GraphicsSystem& GetInstance()
        {
            static GraphicsSystem instance;
            return instance;
        }

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
        void Update();

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

        ShaderSystem* shaderManager;
        ModelLoader* modelLoader;
        TextureLoader* textureLoader;
        ModelManager* modelManager;
        LightSystem* lightManager;
        DebugManager* debugManager;
        CameraSystem* cameraManager;
    };
}