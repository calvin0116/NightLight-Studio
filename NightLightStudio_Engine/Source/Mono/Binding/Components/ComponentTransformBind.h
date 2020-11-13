#pragma once
#include "../../../Component/ComponentTransform.h"
#include <mono/metadata/object.h>

namespace ComponentTransformBind
{
  MonoString* get_Name_Internal(TransformComponent* trans);
  void set_Name_Internal(TransformComponent* trans, MonoString* val);

  MonoObject* get_Position_Internal(TransformComponent* trans);
  void set_Position_Internal(TransformComponent* trans, MonoObject* val);

  MonoObject* get_Rotation_Internal(TransformComponent* trans);
  void set_Rotation_Internal(TransformComponent* trans, MonoObject* val);

  MonoObject* get_Scale_Internal(TransformComponent* trans);
  void set_Scale_Internal(TransformComponent* trans, MonoObject* val);

  void BindComponentTransform();
}
