#include "ComponentVariablesBind.h"
#include "../../MonoWrapper.h"


namespace ComponentVariablesBind
{
  void BindComponentVariables()
  {
    MonoWrapper::BindClassFunction(get_Int_Internal, "get_Int_Internal",
      "Variables");
    MonoWrapper::BindClassFunction(set_Int_Internal, "set_Int_Internal",
      "Variables");

    MonoWrapper::BindClassFunction(get_String_Internal, "get_String_Internal",
      "Variables");
    MonoWrapper::BindClassFunction(set_String_Internal, "set_String_Internal",
      "Variables");

    MonoWrapper::BindClassFunction(get_Float_Internal, "get_Float_Internal",
      "Variables");
    MonoWrapper::BindClassFunction(set_Float_Internal, "set_Float_Internal",
      "Variables");
  }

  int get_Int_Internal(ComponentVariables* var, int index)
  {
    return var->int_list.at(index);
  }

  void set_Int_Internal(ComponentVariables* var, int val, int index)
  {
    var->int_list.at(index) = val;
  }

  MonoString* get_String_Internal(ComponentVariables* var, int index)
  {
    return MonoWrapper::ToMonoString(var->string_list.at(index));
  }

  void set_String_Internal(ComponentVariables* var, MonoString* val, int index)
  {
    var->string_list.at(index) = MonoWrapper::ToString(val);
  }

  float get_Float_Internal(ComponentVariables* var, int index)
  {
    return var->float_list.at(index);
  }

  void set_Float_Internal(ComponentVariables* var, float val, int index)
  {
    var->float_list.at(index) = val;
  }
}
