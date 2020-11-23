#include "InputBind.h"
#include "..\..\MonoWrapper.h" // BindClassFunction
#include "..\..\..\Input\SystemInput.h" // C++ source function


namespace InputBind
{
  void BindInput()
  {
    MonoWrapper::BindClassFunction(csGetKeyPress, "GetKeyPress",
      "Input");
    MonoWrapper::BindClassFunction(csGetKeyHold, "GetKeyHold",
      "Input");
    MonoWrapper::BindClassFunction(csGetKeyUp, "GetKeyUp",
      "Input");
  }

  bool csGetKeyPress(SystemInput_ns::VirtualKey val)
  {
    if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(val))
      return true;
    return false;
  }

  bool csGetKeyHold(SystemInput_ns::VirtualKey val)
  {
    if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(val))
      return true;
    return false;
  }

  bool csGetKeyUp(SystemInput_ns::VirtualKey val)
  {
    if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(val))
      return true;
    return false;
  }
}