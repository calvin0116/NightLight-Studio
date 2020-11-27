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

    MonoWrapper::BindClassFunction(get_Force_Internal, "get_Force_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_Force_Internal, "set_Force_Internal",
      "RigidBody");

    MonoWrapper::BindClassFunction(get_Accel_Internal, "get_Accel_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_Accel_Internal, "set_Accel_Internal",
      "RigidBody");

    MonoWrapper::BindClassFunction(get_Vel_Internal, "get_Vel_Internal",
      "RigidBody");
    MonoWrapper::BindClassFunction(set_Vel_Internal, "set_Vel_Internal",
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

  MonoObject* get_Force_Internal(ComponentRigidBody* rb)
  {
    MonoObject* monoForce = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue(monoForce, "X", rb->force.x);
    MonoWrapper::SetObjectFieldValue(monoForce, "Y", rb->force.y);
    MonoWrapper::SetObjectFieldValue(monoForce, "Z", rb->force.z);
    return monoForce;
  }

  void set_Force_Internal(ComponentRigidBody* rb, MonoObject* force)
  {
    NlMath::Vector3D& myForce = rb->force;
    myForce.x = MonoWrapper::GetObjectFieldValue<float>(force, "X");
    myForce.y = MonoWrapper::GetObjectFieldValue<float>(force, "Y");
    myForce.z = MonoWrapper::GetObjectFieldValue<float>(force, "Z");
  }

  MonoObject* get_Accel_Internal(ComponentRigidBody* rb)
  {
    MonoObject* monoAccel = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue(monoAccel, "X", rb->acceleration.x);
    MonoWrapper::SetObjectFieldValue(monoAccel, "Y", rb->acceleration.y);
    MonoWrapper::SetObjectFieldValue(monoAccel, "Z", rb->acceleration.z);
    return monoAccel;
  }
  void set_Accel_Internal(ComponentRigidBody* rb, MonoObject* accel)
  {
    NlMath::Vector3D& myAccel = rb->acceleration;
    myAccel.x = MonoWrapper::GetObjectFieldValue<float>(accel, "X");
    myAccel.y = MonoWrapper::GetObjectFieldValue<float>(accel, "Y");
    myAccel.z = MonoWrapper::GetObjectFieldValue<float>(accel, "Z");
  }

  MonoObject* get_Vel_Internal(ComponentRigidBody* rb)
  {
    MonoObject* monoVel = MonoWrapper::ConstructObject("Vector3");
    MonoWrapper::SetObjectFieldValue(monoVel, "X", rb->velocity.x);
    MonoWrapper::SetObjectFieldValue(monoVel, "Y", rb->velocity.y);
    MonoWrapper::SetObjectFieldValue(monoVel, "Z", rb->velocity.z);
    return monoVel;
  }

  void set_Vel_Internal(ComponentRigidBody* rb, MonoObject* vel)
  {
    NlMath::Vector3D& myVel = rb->velocity;
    myVel.x = MonoWrapper::GetObjectFieldValue<float>(vel, "X");
    myVel.y = MonoWrapper::GetObjectFieldValue<float>(vel, "Y");
    myVel.z = MonoWrapper::GetObjectFieldValue<float>(vel, "Z");
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
