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
    float offsetX, offsetY;
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
    void SetTargetOffsetXY(float x, float y)
    {
        offsetX = x;
        offsetY = y;
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
      offsetX(0.0f), 
      offsetY(0.0f),
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

            // cam pos
            //   o<- (get the cam pos)
            //      \
            //       \ dist (set by user)
            //        \ view vec (set by camera sys)
            //         \
            //          x tgt (set by user)


            // calculate offset
            //    <---- move 
            //  o<-   o<-
            //    \     \
            //     \     \ 
            //   v  \     \ view vec 
            //       \     \
            //       -------x tgt
            //       ^
            //       offset

            // move the view vect to v where v is the new view vector, v remains the same only tgt changes

            // move vertex (tgt) along the plane
            //
            //          ^       ^  this normal n is the view vector
            //          |  -->  |
            //          |       |
            //     -----x-------x------ le plane
            //        tgt  --> tgt new


            glm::vec3 y_upVector = camSys.GetUpVector();
            glm::vec3 x_sideVector = camSys.GetRightVector();

            glm::vec3 offset = y_upVector * offsetY;
            offset = offset + x_sideVector * -offsetX;

            glm::vec3 targetWoffset = tgt + offset;

            //if(canRotate)
                camSys.SetThridPersonCamTarget(targetWoffset);
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



            ComponentTransform* compTrans = MyID.getComponent<ComponentTransform>();
            glm::vec3 viewVector = camSys.GetViewVector();
            compTrans->_position = targetWoffset - viewVector * dist;


            // rotation?
            // how to check the view vector rot?
        }

    };

    virtual void Exit() override
    {

        NS_GRAPHICS::CameraSystem::GetInstance().SetUseThridPersonCam(false);
    };

    virtual void OnCollisionEnter(Entity) override
    {

    };

  };
} // NS