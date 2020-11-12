#pragma once

#include "../../framework.h"
#include "../Component/ComponentManager.h"

#include "../glm/glm.hpp"   // glm::mat4

#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../Window/WndSystem.h"
#include "../Window/WndUtils.h"

#include "TextureManager.h"
#include "ShaderSystem.h"

namespace NS_GRAPHICS
{
    struct ImageData
    {
        glm::vec3 _position;
        glm::vec2 _uv;
    };

    class UISystem
    {
        UISystem();
        ~UISystem();

    public:
        static UISystem& GetInstance()
        {
            static UISystem instance;
            return instance;
        }

        void Init();
        void Update();
        void RenderUI();

        // Sets ortho projection for UI
        void SetUIMatrix(const int& width, const int& height, const float& near_plane = 1.0f, const float& far_plane = 50000.f);
        unsigned LoadTexture(std::string filename);

    private:

        bool _uiDrawing;

        // Should NOT be calculated every frame
        glm::mat4 _uiMatrix;

        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;
        GLuint _mmbo;

        TextureManager* _textureManager;
        ShaderSystem* _shaderSystem;
    };
}