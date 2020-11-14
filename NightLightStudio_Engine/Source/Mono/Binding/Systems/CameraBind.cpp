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
  //// RIGHT vector
  //MonoObject* csGetRightVector()
  //{

  //}
  //// View Vectors
  //// get viewVector
  //MonoObject* csGetViewVector()
  //{

  //}
  //// get viewVector on XZ plane                     
  //MonoObject* csGetXZViewVector()
  //{

  //}
  //// get viewVector on XZ plane, rotate @Y 180 deg  
  //MonoObject* csGetXZViewVector_Back()
  //{

  //}
  //// get viewVector on XZ plane, rotate @Y 90 deg   
  //MonoObject* csGetXZViewVector_Left()
  //{

  //}
  //// get viewVector on XZ plane, rotate @Y -90 deg  
  //MonoObject* csGetXZViewVector_Right()
  //{

  //}
}