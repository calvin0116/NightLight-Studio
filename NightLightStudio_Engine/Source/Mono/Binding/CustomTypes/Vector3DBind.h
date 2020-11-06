#pragma once
#include "..\..\..\Math\Vector.h"


namespace Vector3DBind
{
  void BindVector3D();

  float get_Axis_Internal(const NlMath::Vector3D* vec, int index);
  void set_Axis_Internal(NlMath::Vector3D* vec, float val, int index);
}