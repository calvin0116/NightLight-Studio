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
#include "CameraSystem.h"

#include <vector>
#include "Emitter.h"
#include "..\Component\ComponentEmitter.h"

#define DEFAULT_MAX_EMITTER 128

namespace NS_GRAPHICS
{
    class EmitterSystem
    {
        EmitterSystem();
        ~EmitterSystem();

    public:
        static EmitterSystem& GetInstance()
        {
            static EmitterSystem instance;
            return instance;
        }

        bool _particleDrawing;
        bool _isPlaying;

        void Init();
        void Update();
        void UpdateEmitter(ComponentEmitter* emitter, float dt);
        void Render(ComponentEmitter* emitter);

        unsigned LoadTexture(std::string filename);

        std::vector<Emitter*> _emitters;
        std::vector<bool> _usedStatus;

        unsigned AddEmitter();
        bool RemoveEmitterByID(const int& index);

        size_t GetFreeIndex();

    private:
        //// Referenced object for instancing
        //GLuint _vao;
        //GLuint _vbo;
        //GLuint _ebo;

        //// Individual elements
        //GLuint _mmbo;
        ////GLuint _cbo;

        TextureManager* _textureManager;
        ShaderSystem* _shaderSystem;
        CameraSystem* _cameraSystem;
    };
}