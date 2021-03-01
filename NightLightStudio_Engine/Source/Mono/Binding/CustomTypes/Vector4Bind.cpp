#include "Vector4Bind.h"
#include "..\..\MonoWrapper.h"


namespace Vector4Bind
{
  void BindVector4()
  {
    MonoWrapper::BindClassFunction(get_xAxis_Internal, "get_xAxis_Internal", "Vector4");
    MonoWrapper::BindClassFunction(set_xAxis_Internal, "set_xAxis_Internal", "Vector4");

    MonoWrapper::BindClassFunction(get_yAxis_Internal, "get_yAxis_Internal", "Vector4");
    MonoWrapper::BindClassFunction(set_yAxis_Internal, "set_yAxis_Internal", "Vector4");

    MonoWrapper::BindClassFunction(get_zAxis_Internal, "get_zAxis_Internal", "Vector4");
    MonoWrapper::BindClassFunction(set_zAxis_Internal, "set_zAxis_Internal", "Vector4");

    MonoWrapper::BindClassFunction(get_wAxis_Internal, "get_wAxis_Internal", "Vector4");
    MonoWrapper::BindClassFunction(set_wAxis_Internal, "set_wAxis_Internal", "Vector4");
  }

  float get_xAxis_Internal(const glm::vec4* vec)
  {
    return vec->x;
  }

  void set_xAxis_Internal(glm::vec4* vec, float val)
  {
    vec->x = val;
  }

  float get_yAxis_Internal(const glm::vec4* vec)
  {
    return vec->y;
  }

  void set_yAxis_Internal(glm::vec4* vec, float val)
  {
    vec->y = val;
  }

  float get_zAxis_Internal(const glm::vec4* vec)
  {
    return vec->z;
  }

  void set_zAxis_Internal(glm::vec4* vec, float val)
  {
    vec->z = val;
  }

  float get_wAxis_Internal(const glm::vec4* vec)
  {
    return vec->w;
  }

  void set_wAxis_Internal(glm::vec4* vec, float val)
  {
    vec->w = val;
  }
}