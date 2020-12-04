#include "UIElementBind.h"
#include "..\..\MonoWrapper.h"


namespace UIElementBind
{
  void BindUIElement()
  {
    MonoWrapper::BindClassFunction(OnClick_Internal, "OnClick_Internal",
      "UIElement");
  }

  bool OnClick_Internal(const UI_Element* ui)
  {
    return ui->OnClick();
  }
}