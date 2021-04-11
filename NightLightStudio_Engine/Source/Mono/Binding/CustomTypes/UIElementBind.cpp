#include "UIElementBind.h"
#include "..\..\MonoWrapper.h"


namespace UIElementBind
{
  void BindUIElement()
  {
    MonoWrapper::BindClassFunction(get_IsActive_Internal, "get_IsActive_Internal",
      "UIElement");
    MonoWrapper::BindClassFunction(set_IsActive_Internal, "set_IsActive_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(get_IsAnimated_Internal, "get_IsAnimated_Internal", "UIElement");
    MonoWrapper::BindClassFunction(set_IsAnimated_Internal, "set_IsAnimated_Internal", "UIElement");

    MonoWrapper::BindClassFunction(get_Width_Internal, "get_Width_Internal",
      "UIElement");
    MonoWrapper::BindClassFunction(set_Width_Internal, "set_Width_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(get_Height_Internal, "get_Height_Internal",
      "UIElement");
    MonoWrapper::BindClassFunction(set_Height_Internal, "set_Height_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(get_Alpha_Internal, "get_Alpha_Internal",
      "UIElement");
    MonoWrapper::BindClassFunction(set_Alpha_Internal, "set_Alpha_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(get_Row_Internal, "get_Row_Internal", "UIElement");
    MonoWrapper::BindClassFunction(set_Row_Internal, "set_Row_Internal", "UIElement");

    MonoWrapper::BindClassFunction(get_Column_Internal, "get_Column_Internal", "UIElement");
    MonoWrapper::BindClassFunction(set_Column_Internal, "set_Column_Internal", "UIElement");

    MonoWrapper::BindClassFunction(get_TotalFrame_Internal, "get_TotalFrame_Internal", "UIElement");
    MonoWrapper::BindClassFunction(set_TotalFrame_Internal, "set_TotalFrame_Internal", "UIElement");

    MonoWrapper::BindClassFunction(get_FramesPerSecond_Internal, "get_FramesPerSecond_Internal", "UIElement");
    MonoWrapper::BindClassFunction(set_FramesPerSecond_Internal, "set_FramesPerSecond_Internal", "UIElement");

    MonoWrapper::BindClassFunction(get_Play_Internal, "get_Play_Internal", "UIElement");
    MonoWrapper::BindClassFunction(set_Play_Internal, "set_Play_Internal", "UIElement");

    MonoWrapper::BindClassFunction(get_Loop_Internal, "get_Loop_Internal", "UIElement");
    MonoWrapper::BindClassFunction(set_Loop_Internal, "set_Loop_Internal", "UIElement");

    MonoWrapper::BindClassFunction(AddTexture_Internal, "AddTexture_Internal", "UIElement");

    MonoWrapper::BindClassFunction(PlayAnimation_Internal, "PlayAnimation_Internal", "UIElement");

    MonoWrapper::BindClassFunction(StopAnimation_Internal, "StopAnimation_Internal", "UIElement");

    MonoWrapper::BindClassFunction(get_Position_Internal, "get_Position_Internal",
      "UIElement");
    MonoWrapper::BindClassFunction(set_csPosition_Internal, "set_Position_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(get_Colour_Internal, "get_Colour_Internal",
      "UIElement");
    MonoWrapper::BindClassFunction(set_myColour_Internal, "set_Colour_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(OnClick_Internal, "OnClick_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(OnHover_Internal, "OnHover_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(OnEnter_Internal, "OnEnter_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(OnExit_Internal, "OnExit_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(OnRelease_Internal, "OnRelease_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(SetFrame_Internal, "SetFrame_Internal",
      "UIElement");

    MonoWrapper::BindClassFunction(CheckIfLastFrame_Internal, "CheckIfLastFrame_Internal",
      "UIElement");
  }

  bool get_IsActive_Internal(const UI_Element* ui)
  {
    return ui->_isActive;
  }
  void set_IsActive_Internal(UI_Element* ui, bool val)
  {
    ui->_isActive = val;
  }

  bool get_IsAnimated_Internal(const UI_Element* ui)
  {
    return ui->_isAnimated;
  }
  void set_IsAnimated_Internal(UI_Element* ui, bool val)
  {
    ui->_isAnimated = val;
  }

  float get_Width_Internal(const UI_Element* ui)
  {
    return ui->_size.x;
  }
  void set_Width_Internal(UI_Element* ui, float val)
  {
    ui->_size.x = val;
  }

  float get_Height_Internal(const UI_Element* ui)
  {
    return ui->_size.y;
  }
  void set_Height_Internal(UI_Element* ui, float val)
  {
    ui->_size.y = val;
  }

  float get_Alpha_Internal(const UI_Element* ui)
  {
    return ui->_colour.a;
  }
  void set_Alpha_Internal(UI_Element* ui, float val)
  {
    ui->_colour.a = val;
  }

  unsigned int get_Row_Internal(const UI_Element* ui)
  {
    return ui->_row;
  }
  void set_Row_Internal(UI_Element* ui, unsigned int val)
  {
    ui->_row = val;
  }

  unsigned int get_Column_Internal(const UI_Element* ui)
  {
    return ui->_column;
  }
  void set_Column_Internal(UI_Element* ui, unsigned int val)
  {
    ui->_column = val;
  }

  unsigned int get_TotalFrame_Internal(const UI_Element* ui)
  {
    return ui->_totalFrame;
  }
  void set_TotalFrame_Internal(UI_Element* ui, unsigned int val)
  {
    ui->_totalFrame = val;
  }

  unsigned int get_FramesPerSecond_Internal(const UI_Element* ui)
  {
    return ui->_framesPerSecond;
  }
  void set_FramesPerSecond_Internal(UI_Element* ui, unsigned int val)
  {
    ui->_framesPerSecond = val;
  }

  bool get_Play_Internal(const UI_Element* ui)
  {
    return ui->_play;
  }
  void set_Play_Internal(UI_Element* ui, bool val)
  {
    ui->_play = val;
  }

  bool get_Loop_Internal(const UI_Element* ui)
  {
    return ui->_loop;
  }
  void set_Loop_Internal(UI_Element* ui, bool val)
  {
    ui->_loop = val;
  }

  void AddTexture_Internal(UI_Element* ui, MonoString* fileName)
  {
    ui->AddTexture(MonoWrapper::ToString(fileName));
  }

  void PlayAnimation_Internal(UI_Element* ui, bool loop, bool reverse, bool current)
  {
    ui->PlayAnimation(loop, reverse, current);
  }

  void StopAnimation_Internal(UI_Element* ui)
  {
    ui->StopAnimation();
  }

  MonoObject* get_Position_Internal(UI_Element* ui)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector3");
    glm::vec3& vec = ui->_position;
    MonoWrapper::SetObjectFieldValue(monoObj, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoObj, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoObj, "Z", vec.z);
    return monoObj;
  }
  void set_csPosition_Internal(UI_Element* ui, MonoObject* val)
  {
    glm::vec3& vec = ui->_position;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }

  MonoObject* get_Colour_Internal(UI_Element* ui)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector3");
    glm::vec4& vec = ui->_colour;
    MonoWrapper::SetObjectFieldValue(monoObj, "X", vec.r);
    MonoWrapper::SetObjectFieldValue(monoObj, "Y", vec.g);
    MonoWrapper::SetObjectFieldValue(monoObj, "Z", vec.b);
    return monoObj;
  }
  void set_myColour_Internal(UI_Element* ui, MonoObject* val)
  {
    glm::vec4& vec = ui->_colour;
    vec.r = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.g = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.b = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }

  bool OnClick_Internal(const UI_Element* ui)
  {
    return ui->OnClick();
  }

  bool OnHover_Internal(const UI_Element* ui)
  {
    return ui->OnHover();
  }

  bool OnEnter_Internal(const UI_Element* ui)
  {
    return ui->OnEnter();
  }

  bool OnExit_Internal(const UI_Element* ui)
  {
    return ui->OnExit();
  }

  bool OnRelease_Internal(const UI_Element* ui)
  {
    return ui->OnRelease();
  }

  void SetFrame_Internal(UI_Element* ui, unsigned int index)
  {
    ui->SetFrame(index);
  }

  bool CheckIfLastFrame_Internal(UI_Element* ui)
  {
      return ui->CheckIfLastFrame();
  }
}