#pragma once
// Include all scripts here
//#include "IScript.h"
#include "IScript.h"

// Construct Scripts from string
namespace AllScripts
{
  IScript* Construct(std::string name);
}