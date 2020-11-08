#include "ForceBind.h"
#include "..\..\MonoWrapper.h" // BindClassFunction
#include "..\..\..\Physics\SystemPhysics.h" // C++ source function

namespace ForceBind
{
  void BindForce()
  {
    MonoWrapper::BindClassFunction(csApply, "Apply",
      "Force");
  }

  void csApply(int ent, MonoObject* dir, float mag)
  {
    NlMath::Vec3 vec;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(dir, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(dir, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(dir, "Z");
    NS_PHYSICS::USE_THE_FORCE.addForce(G_ECMANAGER->getEntity(ent), vec, mag);
  }
}