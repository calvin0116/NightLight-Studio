#pragma once
#include "..\..\..\Math\Vector.h"


namespace Vector3DBind
{
  void BindVector3D();

  float get_xAxis_Internal(const NlMath::Vector3D* vec);
  void set_xAxis_Internal(NlMath::Vector3D* vec, float val);

  float get_yAxis_Internal(const NlMath::Vector3D* vec);
  void set_yAxis_Internal(NlMath::Vector3D* vec, float val);

  float get_zAxis_Internal(const NlMath::Vector3D* vec);
  void set_zAxis_Internal(NlMath::Vector3D* vec, float val);
}