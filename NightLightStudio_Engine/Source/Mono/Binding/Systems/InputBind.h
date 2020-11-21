#pragma once
#include <mono/metadata/object.h> //MonoString*
#include "../../../Input/SI_KeyPress.h" // Virtual Key codes


namespace InputBind
{
  void BindInput();

  bool csGetKeyPress(SystemInput_ns::VirtualKey val);
  bool csGetKeyHold(SystemInput_ns::VirtualKey val);
  bool csGetKeyUp(SystemInput_ns::VirtualKey val);
}