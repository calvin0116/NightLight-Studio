#include "ECSBind.h"
#include "../../MonoWrapper.h"
// Get components etc
#include "../../../Component/Components.h" 

namespace ECSBind
{
  void BindECS()
  {
    MonoWrapper::BindClassFunction(GameObjectFind, "GameObjectFind", "UniBehaviour");
  }

  int GameObjectFind(MonoString* _name)
  {
    Entity en = G_ECMANAGER->getEntity(MonoWrapper::ToString(_name));
    return en.getId();
  }

  MonoObject* get_Transform_handle(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("Transform");
    Entity en = G_ECMANAGER->getEntity(id);
    TransformComponent* trans = en.getComponent<TransformComponent>();

    if (trans == nullptr)
      static_assert("get_Transform_handle nullptr");

    MonoObject* monoVec3;
    // Construct and set position(Vec3) native handle and values
    monoVec3 = MonoWrapper::ConstructObject("Vector3");
    if(trans)
      MonoWrapper::SetNativeHandle(monoVec3, &(trans->_position));
    MonoWrapper::SetObjectFieldValue(monoObj, "position", monoVec3);
    
    // Construct and set rotation(Vec3) native handle and values
    monoVec3 = MonoWrapper::ConstructObject("Vector3");
    if(trans)
      MonoWrapper::SetNativeHandle(monoVec3, &(trans->_rotation));
    MonoWrapper::SetObjectFieldValue(monoObj, "rotation", monoVec3);

    // Construct and set rotation(Vec3) native handle and values
    monoVec3 = MonoWrapper::ConstructObject("Vector3");
    if(trans)
      MonoWrapper::SetNativeHandle(monoVec3, &(trans->_scale));
    MonoWrapper::SetObjectFieldValue(monoObj, "scale", monoVec3);

    return monoObj;
  }
}