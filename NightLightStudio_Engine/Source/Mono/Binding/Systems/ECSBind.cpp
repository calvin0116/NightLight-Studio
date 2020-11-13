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
}