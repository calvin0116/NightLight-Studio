#pragma once
// Include all scripts here
#include "IScript.h"
#include "TestScript.h"
#include "../Player.h"
#include "../CameraScript.h"

// Construct Scripts from string
namespace AllScripts
{
  IScript* Construct(std::string name)
  {
    if (name == "TestScript")
        return dynamic_cast<IScript*>(new TestScript());
    else if (name == "Player")
        return dynamic_cast<IScript*>(new Player());
    else if (name == "Camera")
        return dynamic_cast<CameraScript*>(new CameraScript());
  }
}