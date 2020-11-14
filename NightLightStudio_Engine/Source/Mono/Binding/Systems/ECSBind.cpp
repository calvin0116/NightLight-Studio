#include "ECSBind.h"
#include "../../MonoWrapper.h"
// Get components etc
#include "../../../Component/Components.h" 

namespace ECSBind
{
  void BindECS()
  {
    MonoWrapper::BindClassFunction(GameObjectFind, "GameObjectFind", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetTransform, "GetTransform", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetCollider, "GetCollider", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetRigidBody, "GetRigidBody", "UniBehaviour");
  }

  int GameObjectFind(MonoString* _name)
  {
    Entity en = G_ECMANAGER->getEntity(MonoWrapper::ToString(_name));
    return en.getId();
  }

  MonoObject* GetTransform(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("Transform");
    Entity en = G_ECMANAGER->getEntity(id);
    TransformComponent* trans = en.getComponent<TransformComponent>();
    MonoWrapper::SetNativeHandle(monoObj, trans);

    //if (trans == nullptr)
    //  static_assert("get_Transform_handle nullptr");

    //MonoObject* monoVec3;
    //// Construct and set position(Vec3) native handle and values
    //monoVec3 = MonoWrapper::ConstructObject("Vector3");
    //if(trans)
    //  MonoWrapper::SetNativeHandle(monoVec3, &(trans->_position));
    //MonoWrapper::SetObjectFieldValue(monoObj, "position", monoVec3);
    //
    //// Construct and set rotation(Vec3) native handle and values
    //monoVec3 = MonoWrapper::ConstructObject("Vector3");
    //if(trans)
    //  MonoWrapper::SetNativeHandle(monoVec3, &(trans->_rotation));
    //MonoWrapper::SetObjectFieldValue(monoObj, "rotation", monoVec3);

    //// Construct and set rotation(Vec3) native handle and values
    //monoVec3 = MonoWrapper::ConstructObject("Vector3");
    //if(trans)
    //  MonoWrapper::SetNativeHandle(monoVec3, &(trans->_scale));
    //MonoWrapper::SetObjectFieldValue(monoObj, "scale", monoVec3);

    return monoObj;
  }

  MonoObject* GetCollider(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("Collider");
    Entity en = G_ECMANAGER->getEntity(id);
    ColliderComponent* col = en.getComponent<ColliderComponent>();
    MonoWrapper::SetNativeHandle(monoObj, col);

    ////if (trans == nullptr)
    ////  static_assert("get_Transform_handle nullptr");

    //MonoObject* monoVec3;
    //// Construct and set position(Vec3) native handle and values
    //monoVec3 = MonoWrapper::ConstructObject("Vector3");
    //MonoWrapper::SetNativeHandle(monoVec3, &(col->center));
    //MonoWrapper::SetObjectFieldValue(monoObj, "Center", monoVec3);
    //
    //// Construct and set rotation(Vec3) native handle and values
    //monoVec3 = MonoWrapper::ConstructObject("Vector3");
    //MonoWrapper::SetNativeHandle(monoVec3, &(col->extend));
    //MonoWrapper::SetObjectFieldValue(monoObj, "Extend", monoVec3);

    //// Construct and set rotation(Vec3) native handle and values
    //monoVec3 = MonoWrapper::ConstructObject("Vector3");
    //MonoWrapper::SetNativeHandle(monoVec3, &(col->rotation));
    //MonoWrapper::SetObjectFieldValue(monoObj, "Rotation", monoVec3);

    return monoObj;
  }

  MonoObject* GetRigidBody(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("RigidBody");
    Entity en = G_ECMANAGER->getEntity(id);
    RigidBodyComponent* rb = en.getComponent<RigidBodyComponent>();
    MonoWrapper::SetNativeHandle(monoObj, rb);

    return monoObj;
  }
}