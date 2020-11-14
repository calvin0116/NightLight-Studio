#pragma once
#include "../../../Component/ComponentCollider.h"
#include <mono/metadata/object.h>

namespace ComponentColliderBind
{
  bool get_isCollidable_Internal(ComponentCollider* col);
  void set_isCollidable_Internal(ComponentCollider* col, bool val);

  int get_colliderTag_Internal(ComponentCollider* col);
  void set_colliderTag_Internal(ComponentCollider* col, int val);

  MonoObject* get_Center_Internal(ComponentCollider* trans);
  void set_Center_Internal(ComponentCollider* trans, MonoObject* val);

  MonoObject* get_Extend_Internal(ComponentCollider* trans);
  void set_Extend_Internal(ComponentCollider* trans, MonoObject* val);

  MonoObject* get_Rotation_Internal(ComponentCollider* trans);
  void set_Rotation_Internal(ComponentCollider* trans, MonoObject* val);

  void BindComponentCollider();
}
