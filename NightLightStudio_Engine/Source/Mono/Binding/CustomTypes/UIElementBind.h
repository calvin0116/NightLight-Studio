#pragma once
#include "..\..\..\Component\ComponentCanvas.h"
#include <mono/metadata/object.h>


namespace UIElementBind
{
  void BindUIElement();

  bool get_IsActive_Internal(const UI_Element* ui);
  void set_IsActive_Internal(UI_Element* ui, bool val);

  float get_Width_Internal(const UI_Element* ui);
  void set_Width_Internal(UI_Element* ui, float val);

  float get_Height_Internal(const UI_Element* ui);
  void set_Height_Internal(UI_Element* ui, float val);

  float get_Alpha_Internal(const UI_Element* ui);
  void set_Alpha_Internal(UI_Element* ui, float val);

  MonoObject* get_Position_Internal(UI_Element* ui);
  void set_csPosition_Internal(UI_Element* ui, MonoObject* val);

  MonoObject* get_Colour_Internal(UI_Element* ui);
  void set_myColour_Internal(UI_Element* ui, MonoObject* val);

  bool OnClick_Internal(const UI_Element* ui);
}