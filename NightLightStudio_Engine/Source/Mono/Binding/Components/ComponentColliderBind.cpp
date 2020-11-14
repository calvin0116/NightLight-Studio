#include "ComponentColliderBind.h"
#include "../../MonoWrapper.h"


namespace ComponentColliderBind
{
  void BindComponentCollider()
  {
    MonoWrapper::BindClassFunction(get_isCollidable_Internal, "get_isCollidable_Internal",
      "Collider");
    MonoWrapper::BindClassFunction(set_isCollidable_Internal, "set_isCollidable_Internal",
      "Collider");

    MonoWrapper::BindClassFunction(get_colliderTag_Internal, "get_colliderTag_Internal",
      "Collider");
    MonoWrapper::BindClassFunction(set_colliderTag_Internal, "set_colliderTag_Internal",
      "Collider");

    MonoWrapper::BindClassFunction(get_Center_Internal, "get_Center_Internal",
      "Collider");
    MonoWrapper::BindClassFunction(set_Center_Internal, "set_Center_Internal",
      "Collider");

    MonoWrapper::BindClassFunction(get_Extend_Internal, "get_Extend_Internal",
      "Collider");
    MonoWrapper::BindClassFunction(set_Extend_Internal, "set_Extend_Internal",
      "Collider");

    MonoWrapper::BindClassFunction(get_Rotation_Internal, "get_Rotation_Internal",
      "Collider");
    MonoWrapper::BindClassFunction(set_Rotation_Internal, "set_Rotation_Internal",
      "Collider");
  }

  bool get_isCollidable_Internal(ComponentCollider* col)
  {
    return col->isCollidable;
  }

  void set_isCollidable_Internal(ComponentCollider* col, bool val)
  {
    col->isCollidable = val;
  }

  int get_colliderTag_Internal(ComponentCollider* col)
  {
    return col->colliderTag;
  }

  void set_colliderTag_Internal(ComponentCollider* col, int val)
  {
    col->colliderTag = val;
  }

  MonoObject* get_Center_Internal(ComponentCollider* trans)
  {
    MonoObject* vec = MonoWrapper::ConstructObject("Vector3");
    NlMath::Vector3D& myvec = trans->center;
    MonoWrapper::SetObjectFieldValue(vec, "X", myvec.x);
    MonoWrapper::SetObjectFieldValue(vec, "Y", myvec.y);
    MonoWrapper::SetObjectFieldValue(vec, "Z", myvec.z);
    return vec;
  }

  void set_Center_Internal(ComponentCollider* trans, MonoObject* val)
  {
    NlMath::Vector3D& myvec = trans->center;
    myvec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    myvec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    myvec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }

  MonoObject* get_Extend_Internal(ComponentCollider* trans)
  {
    MonoObject* vec = MonoWrapper::ConstructObject("Vector3");
    NlMath::Vector3D& myvec = trans->extend;
    MonoWrapper::SetObjectFieldValue(vec, "X", myvec.x);
    MonoWrapper::SetObjectFieldValue(vec, "Y", myvec.y);
    MonoWrapper::SetObjectFieldValue(vec, "Z", myvec.z);
    return vec;
  }

  void set_Extend_Internal(ComponentCollider* trans, MonoObject* val)
  {
    NlMath::Vector3D& myvec = trans->extend;
    myvec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    myvec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    myvec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }

  MonoObject* get_Rotation_Internal(ComponentCollider* trans)
  {
    MonoObject* vec = MonoWrapper::ConstructObject("Vector3");
    NlMath::Vector3D& myvec = trans->rotation;
    MonoWrapper::SetObjectFieldValue(vec, "X", myvec.x);
    MonoWrapper::SetObjectFieldValue(vec, "Y", myvec.y);
    MonoWrapper::SetObjectFieldValue(vec, "Z", myvec.z);
    return vec;
  }

  void set_Rotation_Internal(ComponentCollider* trans, MonoObject* val)
  {
    NlMath::Vector3D& myvec = trans->rotation;
    myvec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    myvec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    myvec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }
}
