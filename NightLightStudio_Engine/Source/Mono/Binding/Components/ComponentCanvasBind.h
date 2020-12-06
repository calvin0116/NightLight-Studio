#pragma once
#include "../../../Component/ComponentCanvas.h"
#include <mono/metadata/object.h>

namespace ComponentCanvasBind
{
  bool get_IsActive_Internal(ComponentCanvas* col);
  void set_IsActive_Internal(ComponentCanvas* col, bool val);

  MonoObject* FindUI_Internal(ComponentCanvas* col, unsigned int index);
  MonoObject* FindUIByName_Internal(ComponentCanvas* col, MonoString* name);

  void BindComponentCanvas();
}
