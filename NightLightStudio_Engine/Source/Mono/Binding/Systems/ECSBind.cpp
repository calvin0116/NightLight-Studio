#include "ECSBind.h"
#include "../../MonoWrapper.h"
// Get components etc
#include "../../../Component/Components.h" 
// Delta time
#include "../../../Core/DeltaTime.h"

namespace ECSBind
{
  void BindECS()
  {
    MonoWrapper::BindClassFunction(GameObjectFind, "GameObjectFind", "UniBehaviour");
    MonoWrapper::BindClassFunction(dt, "dt", "UniBehaviour");
    MonoWrapper::BindClassFunction(realDt, "realDt", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetScript, "GetScript", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetTransform, "GetTransform", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetCollider, "GetCollider", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetRigidBody, "GetRigidBody", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetGraphics, "GetGraphics", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetLight, "GetLight", "UniBehaviour");
  }

  int GameObjectFind(MonoString* _name)
  {
    Entity en = G_ECMANAGER->getEntity(MonoWrapper::ToString(_name));
    return en.getId();
  }

  // Delta Time
  float dt()
  {
    return DELTA_T->dt;
  }

  float realDt()
  {
    return DELTA_T->dt / CLOCKS_PER_SEC;
  }

  MonoObject* GetScript(int id)
  {
    Entity en = G_ECMANAGER->getEntity(id);
    ScriptComponent* script = en.getComponent<ScriptComponent>();
    if (script && script->_MonoData._pInstance)
      return script->_MonoData._pInstance; // Found script
    return nullptr; // nope
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

  MonoObject* GetGraphics(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("Graphics");
    Entity en = G_ECMANAGER->getEntity(id);
    GraphicsComponent* rb = en.getComponent<GraphicsComponent>();
    MonoWrapper::SetNativeHandle(monoObj, rb);

    return monoObj;
  }

  MonoObject* GetLight(int id)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Light");
    Entity en = G_ECMANAGER->getEntity(id);
    LightComponent* lc = en.getComponent<LightComponent>();
    MonoWrapper::SetNativeHandle(monoObj, lc);

    return monoObj;
  }
}