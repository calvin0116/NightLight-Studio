#pragma once
// Include all scripts here
#include "IScript.h"
#include "TestScript.h"
#include "../Player.h"

// Construct Scripts from string
namespace AllScripts
{
  IScript* Construct(std::string name)
  {
    if (name == "TestScript")
        return dynamic_cast<IScript*>(new TestScript());
    if (name == "Player")
        return dynamic_cast<IScript*>(new Player());
  }
}