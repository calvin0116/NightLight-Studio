#pragma once
#include "..\..\Core\SceneManager.h"
//#include "..\..\Component\Co"

namespace APIScript
{
  Entity FindEntityByName(std::string name)
  {
    auto& EntityMap = NS_SCENE::SYS_SCENE_MANAGER->EntityName;
    for (auto& [EntityID, Name] : EntityMap)
    {
      if (name == Name)
        return G_ECMANAGER->getEntity(EntityID);
    }
  }
}