#pragma once

#include "../Math/Vector.h"
#include "../Math/Matrix4x4.h"

#include "../Component/ComponentRigidBody.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Physics/SystemPhysics.h"
#include "../Input/SystemInput.h"

#include "../Graphics/GraphicsSystem.h"
#include "../Graphics/CameraSystem.h"

#include "CScripts/IScript.h"

namespace AllScripts
{


class CameraScript : public IScript
{
    glm::vec3 tgt;
    float dist;

public:

    void SetTarget(glm::vec3 _tgt)
    {
        tgt = _tgt;
    }

    void SetDistance(float _dist)
    {
        dist = _dist;
    }

    CameraScript() :
        tgt(0.0f, 0.0f, 0.0f)
    {

    }

    virtual ~CameraScript() override
    {
    }

    virtual void SetEntity(int _id) 
    { 
        _Obj = _id; 
    }

    virtual void Init() override
    {

    };

    virtual void Update() override
    {
        NS_GRAPHICS::CameraSystem& camSys = NS_GRAPHICS::CameraSystem::GetInstance();

        glm::vec3 viewVector = camSys.GetViewVector();

        camSys.SetThridPersonCamTarget(tgt);

        camSys.SetThridPersonCamDistance(dist);

        camSys.SetUseThridPersonCam(true);

    };

    virtual void Exit() override
    {

    };

    virtual void OnCollisionEnter(int other) override
    {

    };

}

} // NS