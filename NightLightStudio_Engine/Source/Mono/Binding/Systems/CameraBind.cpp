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
}