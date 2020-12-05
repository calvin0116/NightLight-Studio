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
  }

  bool get_IsActive_Internal(const UI_Element* ui)
  {
    return ui->_isActive;
  }
  void set_IsActive_Internal(UI_Element* ui, bool val)
  {
    ui->_isActive = val;
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
}