#pragma once
#include "..\..\..\glm\glm.hpp"
#include <mono/metadata/object.h>


namespace Vector4Bind
{
  void BindVector4();

  float get_xAxis_Internal(const glm::vec4* vec);
  void set_xAxis_Internal(glm::vec4* vec, float val);

  float get_yAxis_Internal(const glm::vec4* vec);
  void set_yAxis_Internal(glm::vec4* vec, float val);

  float get_zAxis_Internal(const glm::vec4* vec);
  void set_zAxis_Internal(glm::vec4* vec, float val);

  float get_wAxis_Internal(const glm::vec4* vec);
  void set_wAxis_Internal(glm::vec4* vec, float val);
}