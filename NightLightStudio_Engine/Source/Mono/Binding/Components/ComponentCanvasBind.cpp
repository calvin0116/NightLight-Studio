#include "ComponentCanvasBind.h"
#include "../../MonoWrapper.h"


namespace ComponentCanvasBind
{
  void BindComponentCanvas()
  {
    MonoWrapper::BindClassFunction(get_IsActive_Internal, "get_IsActive_Internal",
      "Canvas");
    MonoWrapper::BindClassFunction(set_IsActive_Internal, "set_IsActive_Internal",
      "Canvas");

    MonoWrapper::BindClassFunction(AddUI_Internal, "AddUI_Internal",
      "Canvas");
    MonoWrapper::BindClassFunction(RemoveUI_Internal, "RemoveUI_Internal",
      "Canvas");
    MonoWrapper::BindClassFunction(csSortUI_Internal, "SortUI_Internal",
      "Canvas");

    MonoWrapper::BindClassFunction(FindUI_Internal, "FindUI_Internal",
      "Canvas");
    MonoWrapper::BindClassFunction(FindUIByName_Internal, "FindUIByName_Internal",
      "Canvas");
  }

  bool get_IsActive_Internal(ComponentCanvas* col)
  {
    return col->_isActive;
  }

  void set_IsActive_Internal(ComponentCanvas* col, bool val)
  {
    col->_isActive = val;
  }
  
  void AddUI_Internal(ComponentCanvas* com)
  {
    com->AddUI();
  }

  void RemoveUI_Internal(ComponentCanvas* com, unsigned int index)
  {
    com->RemoveUI(index);
  }

  void csSortUI_Internal(ComponentCanvas* com)
  {
    com->Sort();
  }

  MonoObject* FindUI_Internal(ComponentCanvas* col, unsigned int index)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("UIElement");
    UI_Element* obj = &(col->FindUI(index));
    //MonoWrapper::SetNativeHandle(monoObj, &col->FindUI(index));
    MonoWrapper::SetNativeHandle(monoObj, obj);
    return monoObj;
  }

  MonoObject* FindUIByName_Internal(ComponentCanvas* col, MonoString* name)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("UIElement");
    MonoWrapper::SetNativeHandle(monoObj, &col->FindUIByName(MonoWrapper::ToString(name)));
    return monoObj;
  }
}
