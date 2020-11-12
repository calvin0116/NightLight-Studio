#pragma once
// Include all scripts here
//#include "IScript.h"
#include "IScript.h"

// Construct Scripts from string
namespace AllScripts
{
  IScript* MyConstruct(std::string name);
}