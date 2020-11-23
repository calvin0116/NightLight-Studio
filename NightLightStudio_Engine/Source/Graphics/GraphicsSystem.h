#pragma once

#include "../Core/MySystem.h"
#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "../Component/ComponentManager.h"

#include "ShaderSystem.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "ModelManager.h"
#include "LightSystem.h"
#include "DebugManager.h"
#include "CameraSystem.h"
#include "TextureManager.h"
#include "UISystem.h"

#include "../glm/glm.hpp"   // glm::mat4


namespace NS_GRAPHICS
{
    class GraphicsSystem : public MySystem, public Singleton<GraphicsSystem>
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
        void Init() override;

        void Exit() override;

        void Render();

        void ToggleDebugDraw(const bool& set);

        // Sets view matrix, called for as long as camera is required to move
        // Default values are set if not called throughout program lifetime
        void SetViewMatrix();

        // Sets perspective frustrum, very expensive, DO NOT CALL EVERY FRAME
        // Default values are set if not called throughout program lifetime
        // fov must be given in degrees
        // DO NOT, DO NOT CHANGE NEAR PLANE
        void SetProjectionMatrix(const float& fov = 45.f, const float& aspect_ratio = 1.78f, const float& near_plane = 1.0f, const float& far_plane = 50000.f);

        // Sets ortho projection for UI
        void SetUIMatrix(const int& width, const int& height, const float& near_plane = -100.0f, const float& far_plane = 100.f);

        // Updates uniform light variables based on active light components for all shaders
        // Temporarily hard set 1 type of light each
        void UpdateLights();

        /////////////////////////////////////////////////////////////////////
        ////// DEBUG FUNCTIONS
        // Creates a basic cube based on given data
        // Attaches graphics component to given object ID
        // Object ID MUST BE VALID, else undefined behavior
        void CreateCube(Entity& entity, const glm::vec3& rgb = glm::vec3(0.5f, 0.5f, 1.0f));

        // Uses preloaded meshes to load sphere mesh instances
        void CreateSphere(Entity& entity, const glm::vec3& rgb = glm::vec3(0.5f, 0.5f, 1.0f));

        // uses preloaded meshes to load cylinder mesh instances
        void CreateCylinder(Entity& entity, const glm::vec3& rgb = glm::vec3(0.5f, 0.5f, 1.0f));

        // Testing for drawing cube without component system
        // Returns mesh ID
        //unsigned TestCreateCube(const glm::vec3& rgb = glm::vec3(0.5f, 0.5f, 0.5f), const float& midExtent = 1.f);
        void SetLineThickness(const float& thickness = 4.f);

        void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& rgb = glm::vec3(0.f,0.f,0.f));

        /////////////////////////////////////////////////////////////////////

        // Sets whole mesh color
        void SetMeshColor(Entity& entity, const glm::vec3& rgb);

        // Loads and stores mesh in local storage, calls mesh loader
        void LoadModel(const std::string& path);

        // Links mesh to specified entity for rendering
        void AttachModel(Entity& entity, const std::string& modelName);

        // Unlinks mesh to specified entity for rendering, if any
        void DetachMesh(Entity& entity);

        glm::mat4 GetInverseViewMatrix();

        void SetHDRTexture(const std::string& filename);

    private:

        bool _hasInit;
        bool _debugDrawing;
        bool _uiDrawing;

        ShaderSystem* shaderManager;
        ModelLoader* modelLoader;
        TextureLoader* textureLoader;
        ModelManager* modelManager;
        LightSystem* lightManager;
        DebugManager* debugManager;
        CameraSystem* cameraManager;
        TextureManager* textureManager;
        UISystem* uiManager;

        //1 HDR PER SCENE
        unsigned _hdrID;

        // Should NOT be calculated every frame
        glm::mat4 _projectionMatrix;

        //temporary view matrix, to be removed after camera system implementation
        glm::mat4 _viewMatrix;

        // Should NOT be calculated every frame
        glm::mat4 _orthoMatrix;
    };

    //GLOBAL pointer to an instance of graphic system
    //== Good to have for every system so that you dont need to always get instance
    static GraphicsSystem* SYS_GRAPHICS = GraphicsSystem::GetInstance();
}