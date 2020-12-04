#pragma once
#include "../../../Component/ComponentCanvas.h"
#include <mono/metadata/object.h>

namespace ComponentCanvasBind
{
  MonoObject* FindUI_Internal(ComponentCanvas* col, unsigned int index);
  MonoObject* FindUIByName_Internal(ComponentCanvas* col, MonoString* name);

  void BindComponentCanvas();
}
