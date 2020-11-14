#include "Vector3DBind.h"
#include "..\..\MonoWrapper.h"


namespace Vector3DBind
{
  void BindVector3D()
  {
    MonoWrapper::BindClassFunction(get_xAxis_Internal, "get_xAxis_Internal", "Vector3");
    MonoWrapper::BindClassFunction(set_xAxis_Internal, "set_xAxis_Internal", "Vector3");

    MonoWrapper::BindClassFunction(get_yAxis_Internal, "get_yAxis_Internal", "Vector3");
    MonoWrapper::BindClassFunction(set_yAxis_Internal, "set_yAxis_Internal", "Vector3");

    MonoWrapper::BindClassFunction(get_zAxis_Internal, "get_zAxis_Internal", "Vector3");
    MonoWrapper::BindClassFunction(set_zAxis_Internal, "set_zAxis_Internal", "Vector3");
  }

  float get_xAxis_Internal(const NlMath::Vector3D* vec)
  {
    return vec->x;
  }

  void set_xAxis_Internal(NlMath::Vector3D* vec, float val)
  {
    vec->x = val;
  }

  float get_yAxis_Internal(const NlMath::Vector3D* vec)
  {
    return vec->y;
  }

  void set_yAxis_Internal(NlMath::Vector3D* vec, float val)
  {
    vec->y = val;
  }

  float get_zAxis_Internal(const NlMath::Vector3D* vec)
  {
    return vec->z;
  }

  void set_zAxis_Internal(NlMath::Vector3D* vec, float val)
  {
    vec->z = val;
  }
}