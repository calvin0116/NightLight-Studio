#include "Vector3DBind.h"
#include "..\..\MonoWrapper.h"


namespace Vector3DBind
{
  void BindVector3D()
  {
    MonoWrapper::BindClassFunction(get_Axis_Internal, "get_Axis_Internal", "Vector3");
    MonoWrapper::BindClassFunction(set_Axis_Internal, "set_Axis_Internal", "Vector3");
  }

  float get_Axis_Internal(const NlMath::Vector3D* vec, int index)
  {
    return vec->m[index];
  }

  void set_Axis_Internal(NlMath::Vector3D* vec, float val, int index)
  {
    vec->m[index] = val;
  }
}