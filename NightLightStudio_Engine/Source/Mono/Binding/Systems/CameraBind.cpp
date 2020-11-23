#include "CameraBind.h"
#include "..\..\MonoWrapper.h" // BindClassFunction
#include "..\..\..\Graphics\CameraSystem.h" // C++ source function

namespace CameraBind
{
  void BindCamera()
  {
    MonoWrapper::BindClassFunction(csGetUpVector, "GetUpVector",
      "Camera");
    MonoWrapper::BindClassFunction(csGetRightVector, "GetRightVector",
      "Camera");
    MonoWrapper::BindClassFunction(csGetViewVector, "GetViewVector",
      "Camera");
    MonoWrapper::BindClassFunction(csGetXZViewVector, "GetXZViewVector",
      "Camera");
    MonoWrapper::BindClassFunction(csGetXZViewVector_Back, "GetXZViewVector_Back",
      "Camera");
    MonoWrapper::BindClassFunction(csGetXZViewVector_Left, "GetXZViewVector_Left",
      "Camera");
    MonoWrapper::BindClassFunction(csGetXZViewVector_Right, "GetXZViewVector_Right",
      "Camera");

    // Camera functions
    MonoWrapper::BindClassFunction(csSetUseThirdPersonCam, "SetUseThirdPersonCam",
      "Camera");

    MonoWrapper::BindClassFunction(csToggleUseThirdPersonCam, "ToggleUseThirdPersonCam",
      "Camera");

    MonoWrapper::BindClassFunction(csSetThirdPersonCamCanRotateAnot, "SetThirdPersonCamCanRotateAnot",
      "Camera");

    MonoWrapper::BindClassFunction(csSetThirdPersonCamCanZoomAnot, "SetThirdPersonCamCanZoomAnot",
      "Camera");

    MonoWrapper::BindClassFunction(csSetThirdPersonCamTarget, "SetThirdPersonCamTarget",
      "Camera");

    MonoWrapper::BindClassFunction(csSetThirdPersonCamDistance, "SetThirdPersonCamDistance",
      "Camera");

    MonoWrapper::BindClassFunction(csSetThirdPersonCamPitchAndYaw, "SetThirdPersonCamPitchAndYaw",
      "Camera");
  }

  // Directional Vectors
  // UP vector
  MonoObject* csGetUpVector()
  {
    glm::vec3 Vec3 = NS_GRAPHICS::CameraSystem::GetInstance().GetUpVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetRightVector()
  {
    glm::vec3 Vec3 = NS_GRAPHICS::CameraSystem::GetInstance().GetRightVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetViewVector()
  {
    glm::vec3 Vec3 = NS_GRAPHICS::CameraSystem::GetInstance().GetViewVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector()
  {
    glm::vec3 Vec3 = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector_Back()
  {
    glm::vec3 Vec3 = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Back();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector_Left()
  {
    glm::vec3 Vec3 = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Left();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector_Right()
  {
    glm::vec3 Vec3 = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Right();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  // Camera Functions
  void csSetUseThirdPersonCam(bool set)
  {
    NS_GRAPHICS::CameraSystem::GetInstance().SetUseThridPersonCam(set);
  }

  void csToggleUseThirdPersonCam()
  {
    NS_GRAPHICS::CameraSystem::GetInstance().ToggleUseThridPersonCam();
  }

  void csSetThirdPersonCamCanRotateAnot(bool set)
  {
    NS_GRAPHICS::CameraSystem::GetInstance().SetThridPersonCamCanRotateAnot(set);
  }

  void csSetThirdPersonCamCanZoomAnot(bool set)
  {
    NS_GRAPHICS::CameraSystem::GetInstance().SetThridPersonCamCanZoomAnot(set);
  }

  void csSetThirdPersonCamTarget(MonoObject* tgt)
  {
    glm::vec3 vec;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(tgt, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(tgt, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(tgt, "Z");
    NS_GRAPHICS::CameraSystem::GetInstance().SetThridPersonCamTarget(vec);
  }

  void csSetThirdPersonCamDistance(float dist)
  {
    NS_GRAPHICS::CameraSystem::GetInstance().SetThridPersonCamDistance(dist);
  }

  void csSetThirdPersonCamPitchAndYaw(float pitch, float yaw)
  {
    NS_GRAPHICS::CameraSystem::GetInstance().SetThridPersonCamPitchAndYaw(pitch, yaw);
  }
}