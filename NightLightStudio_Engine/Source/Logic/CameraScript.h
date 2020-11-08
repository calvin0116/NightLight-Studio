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
      //  int  MyID;
    glm::vec3 tgt;
    glm::vec3 initTgt;
    glm::vec3 initPos;
    float dist;
    bool isActive;

    bool canRotate;
    bool canZoom;

  public:

    // set the camera target
    void SetTarget(glm::vec3 _tgt)
    {
      tgt = _tgt;
    }
    // set the camera distance from the target
    void SetDistance(float _dist)
    {
      dist = _dist;
    }
    // set camera active anot - only 1 cam active at a time
    void SetActive(bool _set)
    {
      isActive = _set;
    }
    // set camera can rotate anot
    void SetRotate(bool _set)
    {
      canRotate = _set;
    }
    // set camera can zoom anot
    void SetZoom(bool _set)
    {
        canZoom = _set;
    }

    CameraScript() :
      tgt(0.0f, 0.0f, 0.0f),
      initTgt(0.0f, 0.0f, 0.0f),
      initPos(0.0f, 0.0f, 0.0f),
      dist(1.0f),
      isActive(false),
      canRotate(true),
      canZoom(true)
    {

    }

    virtual ~CameraScript() override
    {
    }

    virtual void SetEntity(Entity _id)
    {
      MyID = _id;
    }

    virtual void Init() override
    {
        // set default target using transform

        ComponentTransform* compTrans = MyID.getComponent<ComponentTransform>();
        
        dist = compTrans->_scale.x;

        glm::vec3 viewVector(dist, 0.0f, 0.0f);
        glm::quat quaternion(glm::radians(compTrans->_rotation));
        glm::mat4 rotate = glm::mat4_cast(quaternion);
        viewVector = rotate * glm::vec4(viewVector, 1.0f);
        viewVector = glm::normalize(viewVector);

        //compTrans->_position = tgt - viewVector * dist;
        tgt = compTrans->_position + viewVector * dist;
        initTgt = tgt;
        //tgt = compTrans->_position + viewVector;
        initPos = compTrans->_position;
    };

    virtual void Update() override
    {
        if (isActive)
        {
            NS_GRAPHICS::CameraSystem& camSys = NS_GRAPHICS::CameraSystem::GetInstance();

            //if(canRotate)
                camSys.SetThridPersonCamTarget(tgt);
            //else
            //    camSys.SetThridPersonCamTarget(initTgt);

            camSys.SetThridPersonCamDistance(dist);

            if (!canRotate)
            {
                glm::vec3 vv = initTgt - initPos;

                camSys.SetThridPersonCamPitchAndYaw(asin(-vv.y), atan2(vv.x, vv.z));
            }

            camSys.SetThridPersonCamCanRotateAnot(canRotate);

            camSys.SetThridPersonCamCanZoomAnot(canZoom);

            camSys.SetUseThridPersonCam(true);

            // cam pos
            //   o<- (get the cam pos)
            //      \
            //       \ dist (set by user)
            //        \ view vec (set by camera sys)
            //         \
            //          x tgt (set by user)
            ComponentTransform* compTrans = MyID.getComponent<ComponentTransform>();
            glm::vec3 viewVector = camSys.GetViewVector();
            compTrans->_position = tgt - viewVector * dist;

            // rotation?
            // how to check the view vector rot?
        }

    };

    virtual void Exit() override
    {

    };

    virtual void OnCollisionEnter(Entity) override
    {

    };

  };
} // NS