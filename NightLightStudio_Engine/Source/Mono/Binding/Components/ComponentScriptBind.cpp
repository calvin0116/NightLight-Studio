#include "ComponentScriptBind.h"
#include "../../MonoWrapper.h"


namespace ComponentScriptBind
{
  void BindComponentScript()
  {
    MonoWrapper::BindClassFunction(get_IsActive_Internal, "get_IsActive_Internal",
      "Script");
    MonoWrapper::BindClassFunction(set_IsActive_Internal, "set_IsActive_Internal",
      "Script");
  }

  bool get_IsActive_Internal(ComponentScript* script)
  {
    return script->isActive;
  }

  void set_IsActive_Internal(ComponentScript* script, bool val)
  {
    script->isActive = val;
  }
}
