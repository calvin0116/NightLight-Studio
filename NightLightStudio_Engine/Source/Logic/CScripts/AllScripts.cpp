#pragma once
// Include all scripts here
//#include "IScript.h"
#include "AllScripts.h"
#include "TestScript.h"
#include "TestScript2.h"

#include "../Player.h"
#include "../CameraScript.h"
#include "PossessScript.h"
#include "../SpawnPoint.h"
#include "FanScript.h"
#include "FanBlowScript.h"
#include "CauldronScript.h"

// Construct Scripts from string
namespace AllScripts
{
  IScript* MyConstruct(std::string name)
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
    else if (name == "FanScript")
      return dynamic_cast<IScript*>(new FanScript());
    else if (name == "FanBlowScript")
      return dynamic_cast<IScript*>(new FanBlowScript());
    else if (name == "CauldronScript")
      return dynamic_cast<IScript*>(new CauldronScript());
    return nullptr;
  }
}