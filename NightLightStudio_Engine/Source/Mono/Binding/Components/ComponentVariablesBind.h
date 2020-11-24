#pragma once
#include "../../../Component/ComponentVariables.h"
#include <mono/metadata/object.h>

namespace ComponentVariablesBind
{
  int get_Int_Internal(ComponentVariables* var, int index);
  void set_Int_Internal(ComponentVariables* var, int val, int index);

  MonoString* get_String_Internal(ComponentVariables* var, int index);
  void set_String_Internal(ComponentVariables* var, MonoString* val, int index);

  float get_Float_Internal(ComponentVariables* var, int index);
  void set_Float_Internal(ComponentVariables* var, float val, int index);

  void BindComponentVariables();
}
