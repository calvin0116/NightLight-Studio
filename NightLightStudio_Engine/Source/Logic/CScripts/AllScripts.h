#pragma once
// Include all scripts here
//#include "IScript.h"
#include "TestScript.h"
#include "TestScript2.h"

#include "../Player.h"
#include "../CameraScript.h"
#include "PossessScript.h"
#include "../SpawnPoint.h"

// Construct Scripts from string
namespace AllScripts
{
  IScript* Construct(std::string name)
  {
    if (name == "TestScript")
      return dynamic_cast<IScript*>(new TestScript());
    else if (name == "TestScript2")
      return dynamic_cast<IScript*>(new TestScript2());
    else if (name == "Player")
      return dynamic_cast<IScript*>(new Player());
    else if (name == "Camera")
      return dynamic_cast<IScript*>(new CameraScript());
    else if (name == "Possess")
      return dynamic_cast<IScript*>(new PossessScript());
    else if (name == "SpawnPoint")
        return dynamic_cast<IScript*>(new SpawnPoint());
    // No such script
    return nullptr;
  }
}