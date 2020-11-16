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

        //UIQuad _base;

    public:
        static UISystem& GetInstance()
        {
            static UISystem instance;
            return instance;
        }

        void Init();
        void Update();
        void RenderUI();

        unsigned LoadTexture(std::string filename);

    private:

        bool _uiDrawing;

        // Referenced object for instancing
        GLuint _vao;
        GLuint _vbo;
        GLuint _ebo;

        // Individual elements
        GLuint _mmbo;
        //GLuint _cbo;

        TextureManager* _textureManager;
        ShaderSystem* _shaderSystem;
    };
}