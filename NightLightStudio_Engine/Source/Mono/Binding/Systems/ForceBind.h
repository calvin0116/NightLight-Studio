#pragma once
#include <mono/metadata/object.h>
//#include "..\..\..\Math\Vector.h"

namespace ForceBind
{
  void BindForce();

  void csApply(int entity, MonoObject* dir, float magnitude);
}