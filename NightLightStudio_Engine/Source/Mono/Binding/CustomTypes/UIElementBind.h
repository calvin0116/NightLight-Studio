#pragma once
#include "..\..\..\Component\ComponentCanvas.h"


namespace UIElementBind
{
  void BindUIElement();

  bool OnClick_Internal(const UI_Element* ui);
}