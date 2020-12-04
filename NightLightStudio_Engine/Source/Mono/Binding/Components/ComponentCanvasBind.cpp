#include "ComponentCanvasBind.h"
#include "../../MonoWrapper.h"


namespace ComponentCanvasBind
{
  void BindComponentCanvas()
  {
    MonoWrapper::BindClassFunction(FindUI_Internal, "FindUI_Internal",
      "Canvas");
    MonoWrapper::BindClassFunction(FindUIByName_Internal, "FindUIByName_Internal",
      "Canvas");
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
