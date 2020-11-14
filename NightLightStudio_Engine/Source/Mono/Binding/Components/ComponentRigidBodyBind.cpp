#include "ComponentRigidBodyBind.h"
#include "../../MonoWrapper.h"


namespace ComponentRigidBodyBind
{
  void BindComponentRigidBody()
  {
    MonoWrapper::BindClassFunction(get_isActive_Internal, "get_isActive_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_isActive_Internal, "set_isActive_Internal",
      "RigidBody");


    MonoWrapper::BindClassFunction(get_isStatic_Internal, "get_isStatic_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_isStatic_Internal, "set_isStatic_Internal",
      "RigidBody");

    MonoWrapper::BindClassFunction(get_isGravity_Internal, "get_isGravity_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_isGravity_Internal, "set_isGravity_Internal",
      "RigidBody");

    MonoWrapper::BindClassFunction(get_Mass_Internal, "get_Mass_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_Mass_Internal, "set_Mass_Internal",
      "RigidBody");

    MonoWrapper::BindClassFunction(get_Friction_Internal, "get_Friction_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_Friction_Internal, "set_Friction_Internal",
      "RigidBody");
  }

  bool get_isActive_Internal(ComponentRigidBody* rb)
  {
    return rb->isActive;
  }

  void set_isActive_Internal(ComponentRigidBody* rb, bool val)
  {
    rb->isActive = val;
  }

  bool get_isStatic_Internal(ComponentRigidBody* rb)
  {
    return rb->isStatic;
  }

  void set_isStatic_Internal(ComponentRigidBody* rb, bool val)
  {
    rb->isStatic = val;
  }

  bool get_isGravity_Internal(ComponentRigidBody* rb)
  {
    return rb->isGravity;
  }

  void set_isGravity_Internal(ComponentRigidBody* rb, bool val)
  {
    rb->isGravity = val;
  }

  float get_Mass_Internal(ComponentRigidBody* rb)
  {
    return rb->mass;
  }

  void set_Mass_Internal(ComponentRigidBody* rb, float val)
  {
    rb->mass = val;
  }

  float get_Friction_Internal(ComponentRigidBody* rb)
  {
    return rb->friction;
  }

  void set_Friction_Internal(ComponentRigidBody* rb, float val)
  {
    rb->friction = val;
  }
}
