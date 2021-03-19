#pragma once
#include "..\..\..\Component\ComponentCanvas.h"
#include <mono/metadata/object.h>


namespace UIElementBind
{
  void BindUIElement();

  bool get_IsActive_Internal(const UI_Element* ui);
  void set_IsActive_Internal(UI_Element* ui, bool val);

  bool get_IsAnimated_Internal(const UI_Element* ui);
  void set_IsAnimated_Internal(UI_Element* ui, bool val);

  float get_Width_Internal(const UI_Element* ui);
  void set_Width_Internal(UI_Element* ui, float val);

  float get_Height_Internal(const UI_Element* ui);
  void set_Height_Internal(UI_Element* ui, float val);

  float get_Alpha_Internal(const UI_Element* ui);
  void set_Alpha_Internal(UI_Element* ui, float val);

  unsigned int get_Row_Internal(const UI_Element* ui);
  void set_Row_Internal(UI_Element* ui, unsigned int val);

  unsigned int get_Column_Internal(const UI_Element* ui);
  void set_Column_Internal(UI_Element* ui, unsigned int val);

  unsigned int get_TotalFrame_Internal(const UI_Element* ui);
  void set_TotalFrame_Internal(UI_Element* ui, unsigned int val);

  unsigned int get_FramesPerSecond_Internal(const UI_Element* ui);
  void set_FramesPerSecond_Internal(UI_Element* ui, unsigned int val);

  bool get_Play_Internal(const UI_Element* ui);
  void set_Play_Internal(UI_Element* ui, bool val);

  bool get_Loop_Internal(const UI_Element* ui);
  void set_Loop_Internal(UI_Element* ui, bool val);

  void AddTexture_Internal(UI_Element* ui, MonoString* val);

  void PlayAnimation_Internal(UI_Element* ui, bool loop);

  void StopAnimation_Internal(UI_Element* ui);

  MonoObject* get_Position_Internal(UI_Element* ui);
  void set_csPosition_Internal(UI_Element* ui, MonoObject* val);

  MonoObject* get_Colour_Internal(UI_Element* ui);
  void set_myColour_Internal(UI_Element* ui, MonoObject* val);

  bool OnClick_Internal(const UI_Element* ui);

  bool OnHover_Internal(const UI_Element* ui);

  bool OnEnter_Internal(const UI_Element* ui);

  bool OnExit_Internal(const UI_Element* ui);

  bool OnRelease_Internal(const UI_Element* ui);

  void SetFrame_Internal(UI_Element* ui, unsigned int index);

  bool CheckIfLastFrame_Internal(UI_Element* ui);
}