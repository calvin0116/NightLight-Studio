#include "CameraBind.h"
#include "..\..\MonoWrapper.h" // BindClassFunction
#include "..\..\..\Graphics\CameraSystem.h" // C++ source function


namespace CameraBind
{
#define NS_CAM NS_GRAPHICS::CameraSystem
#define Camera() NS_CAM::GetInstance()

  void BindCamera()
  {
    MonoWrapper::BindClassFunction(csGetPosition, "GetPosition",
      "Camera");
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

    // Getter / Setter
    MonoWrapper::BindClassFunction(csGetFOV, "GetFOV",
      "Camera");
    MonoWrapper::BindClassFunction(csSetFOV, "SetFOV",
      "Camera");
  }

  // Directional Vectors
  // Position of camera
  MonoObject* csGetPosition()
  {
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    glm::vec3 vec3 = Camera().GetCurrentCameraPosition();
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", vec3.z);
    return monoVec3;
  }

  // UP vector
  MonoObject* csGetUpVector()
  {
    glm::vec3 Vec3 = Camera().GetUpVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetRightVector()
  {
    glm::vec3 Vec3 = Camera().GetRightVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetViewVector()
  {
    glm::vec3 Vec3 = Camera().GetViewVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector()
  {
    glm::vec3 Vec3 = Camera().GetXZViewVector();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector_Back()
  {
    glm::vec3 Vec3 = Camera().GetXZViewVector_Back();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector_Left()
  {
    glm::vec3 Vec3 = Camera().GetXZViewVector_Left();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  MonoObject* csGetXZViewVector_Right()
  {
    glm::vec3 Vec3 = Camera().GetXZViewVector_Right();
    MonoObject* monoVec3 = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "X", Vec3.x);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Y", Vec3.y);
    MonoWrapper::SetObjectFieldValue<float>(monoVec3, "Z", Vec3.z);
    return monoVec3;
  }

  // Camera Functions
  void csSetUseThirdPersonCam(bool set)
  {
    Camera().SetUseThridPersonCam(set);
  }

  void csToggleUseThirdPersonCam()
  {
    Camera().ToggleUseThridPersonCam();
  }

  void csSetThirdPersonCamCanRotateAnot(bool set)
  {
    Camera().SetThridPersonCamCanRotateAnot(set);
  }

  void csSetThirdPersonCamCanZoomAnot(bool set)
  {
    Camera().SetThridPersonCamCanZoomAnot(set);
  }

  void csSetThirdPersonCamTarget(MonoObject* tgt)
  {
    glm::vec3 vec;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(tgt, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(tgt, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(tgt, "Z");
    Camera().SetThridPersonCamTarget(vec);
  }

  void csSetThirdPersonCamDistance(float dist)
  {
    Camera().SetThridPersonCamDistance(dist);
  }

  void csSetThirdPersonCamPitchAndYaw(float pitch, float yaw)
  {
    Camera().SetThridPersonCamPitchAndYaw(pitch, yaw);
  }

  float csGetFOV()
  {
    return Camera().GetFOV();
  }

  void csSetFOV(float _fov)
  {
    Camera().SetFOV(_fov);
  }
}