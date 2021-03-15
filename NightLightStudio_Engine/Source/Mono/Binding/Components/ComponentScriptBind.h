#pragma once
#include "../../../Component/ComponentScript.h"
#include <mono/metadata/object.h>

namespace ComponentScriptBind
{
  bool get_IsActive_Internal(ComponentScript* script);
  void set_IsActive_Internal(ComponentScript* script, bool val);

  void BindComponentScript();
}
