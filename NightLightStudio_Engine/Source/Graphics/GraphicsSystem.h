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

#include "../glm/glm.hpp"   // glm::mat4


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

        void ToggleDebugDraw(const bool& set);

        // Sets view matrix, called for as long as camera is required to move
        // Default values are set if not called throughout program lifetime
        void SetViewMatrix();

        // Sets perspective frustrum, very expensive, DO NOT CALL EVERY FRAME
        // Default values are set if not called throughout program lifetime
        // fov must be given in degrees
        void SetProjectionMatrix(const float& fov = 45.f, const float& aspect_ratio = 1.78f, const float& near_plane = 0.01f, const float& far_plane = 1000.f);

        // Creates a basic cube based on given data
        void CreateCube(const glm::vec3& rgb, const float& edgeLength);

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

        // Should NOT be calculated every frame
        glm::mat4 projectionMatrix;

        //temporary view matrix, to be removed after camera system implementation
        glm::mat4 viewMatrix;
    };

    //GLOBAL pointer to an instance of graphic system
    //== Good to have for every system so that you dont need to always get instance
    static GraphicsSystem* SYS_GRAPHICS = GraphicsSystem::GetInstance();
}