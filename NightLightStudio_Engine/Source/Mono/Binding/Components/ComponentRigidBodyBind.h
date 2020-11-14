#pragma once
#include "../../../Component/ComponentRigidBody.h"
#include <mono/metadata/object.h>

namespace ComponentRigidBodyBind
{
  bool get_isActive_Internal(ComponentRigidBody* rb);
  void set_isActive_Internal(ComponentRigidBody* rb, bool val);

  bool get_isStatic_Internal(ComponentRigidBody* rb);
  void set_isStatic_Internal(ComponentRigidBody* rb, bool val);

  bool get_isGravity_Internal(ComponentRigidBody* rb);
  void set_isGravity_Internal(ComponentRigidBody* rb, bool val);

  float get_Mass_Internal(ComponentRigidBody* rb);
  void set_Mass_Internal(ComponentRigidBody* rb, float val);

  float get_Friction_Internal(ComponentRigidBody* rb);
  void set_Friction_Internal(ComponentRigidBody* rb, float val);

  void BindComponentRigidBody();
}
