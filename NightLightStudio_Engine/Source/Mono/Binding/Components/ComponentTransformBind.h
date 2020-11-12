#pragma once
#include "../../../Component/ComponentTransform.h"
#include <mono/metadata/object.h>

namespace ComponentTransformBind
{
  MonoString* get_Name_Internal(TransformComponent* trans);
  void set_Name_Internal(TransformComponent* trans, MonoString* val);

  void BindAudioSource();
}
