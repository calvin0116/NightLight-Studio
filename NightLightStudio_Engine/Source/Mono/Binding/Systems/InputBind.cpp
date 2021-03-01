#include "InputBind.h"
#include "..\..\MonoWrapper.h" // BindClassFunction
#include "..\..\..\Input\SystemInput.h" // C++ source function


namespace InputBind
{
  void BindInput()
  {
    MonoWrapper::BindClassFunction(csGetKeyPress, "GetKeyPress",
      "Input");
    MonoWrapper::BindClassFunction(csGetKeyHold, "GetKeyHold",
      "Input");
    MonoWrapper::BindClassFunction(csGetKeyUp, "GetKeyUp",
      "Input");

    MonoWrapper::BindClassFunction(csResetcontroller, "Resetcontroller",
      "Input");
    MonoWrapper::BindClassFunction(csCTRLButtonDown, "GetCTRLButtonDown",
      "Input");
    MonoWrapper::BindClassFunction(csGetButtonPress, "GetButtonPress",
      "Input");
    MonoWrapper::BindClassFunction(csGetButtonHold, "GetButtonHold",
      "Input");
    MonoWrapper::BindClassFunction(csGetButtonUp, "GetButtonUp",
      "Input");

    MonoWrapper::BindClassFunction(csGetMousePos, "GetMousePos",
      "Input");
    MonoWrapper::BindClassFunction(csGetOriginalMousePos, "GetOriginalMousePos",
      "Input");
    MonoWrapper::BindClassFunction(csGetMouseDragVec, "GetMouseDragVec",
      "Input");
    MonoWrapper::BindClassFunction(csToggleShowCursor, "ToggleShowCursor",
      "Input");
    MonoWrapper::BindClassFunction(csSetCursorVisible, "SetCursorVisible",
      "Input");
    MonoWrapper::BindClassFunction(csLoadCursorImage, "LoadCursorImage",
      "Input");
    MonoWrapper::BindClassFunction(csGetClientRectSize, "GetClientRectSize",
      "Input");
    MonoWrapper::BindClassFunction(csToggleClipCursor, "ToggleClipCursor",
      "Input");
    MonoWrapper::BindClassFunction(csSetClipCursor, "SetClipCursor",
      "Input");
    MonoWrapper::BindClassFunction(csSetCursorPos, "SetCursorPos",
      "Input");
    MonoWrapper::BindClassFunction(csSetToCenter, "SetToCenter",
      "Input");
    MonoWrapper::BindClassFunction(csGetRelativeLocation, "GetRelativeLocation",
      "Input");
    MonoWrapper::BindClassFunction(csGetRelativeDragVec, "GetRelativeDragVec",
      "Input");
    MonoWrapper::BindClassFunction(csSetScroll, "SetScroll",
      "Input");
    MonoWrapper::BindClassFunction(csGetIfScrollUp, "GetScrollUp",
      "Input");
    MonoWrapper::BindClassFunction(csGetIfScrollDown, "GetScrollDown",
      "Input");
    MonoWrapper::BindClassFunction(csResetWinSize, "ResetWinSize",
      "Input");
  }
  // Keyboard
  bool csGetKeyPress(SystemInput_ns::VirtualKey val)
  {
    if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(val))
      return true;
    return false;
  }

  bool csGetKeyHold(SystemInput_ns::VirtualKey val)
  {
    if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(val))
      return true;
    return false;
  }

  bool csGetKeyUp(SystemInput_ns::VirtualKey val)
  {
    if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(val))
      return true;
    return false;
  }
  // Controller
  void csResetcontroller(bool allowController)
  {
    SYS_INPUT->GetSystemController().ResetController(allowController);
  }

  bool csCTRLButtonDown(unsigned int button)
  {
    if (SYS_INPUT->GetSystemController().CTRLButtonDown(button))
      return true;
    return false;
  }

  bool csGetButtonPress(unsigned int button)
  {
    if (SYS_INPUT->GetSystemController().GetIfButtonPress(button))
      return true;
    return false;
  }

  bool csGetButtonHold(unsigned int button)
  {
    if (SYS_INPUT->GetSystemController().GetIfButtonHeld(button))
      return true;
    return false;
  }

  bool csGetButtonUp(unsigned int button)
  {
    if (SYS_INPUT->GetSystemController().GetIfButtonRelease(button))
      return true;
    return false;
  }
  // Mouse
  MonoObject* csGetMousePos()
  {
    MonoObject* vec3 = MonoWrapper::ConstructObject("Vector3");
    glm::vec2 myVec2 = SYS_INPUT->GetSystemMousePos().GetMousePos();
    MonoWrapper::SetObjectFieldValue<float>(vec3, "X", myVec2.x);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Y", myVec2.y);
    float myZ = 0.0f;
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Z", myZ);
    return vec3;
  }

  MonoObject* csGetOriginalMousePos()
  {
    MonoObject* vec3 = MonoWrapper::ConstructObject("Vector3");
    glm::vec2 myVec2 = SYS_INPUT->GetSystemMousePos().GetOriginalMousePos();
    MonoWrapper::SetObjectFieldValue<float>(vec3, "X", myVec2.x);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Y", myVec2.y);
    float myZ = 0.0f;
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Z", myZ);
    return vec3;
  }

  MonoObject* csGetMouseDragVec()
  {
    MonoObject* vec3 = MonoWrapper::ConstructObject("Vector3");
    glm::vec2 myVec2 = SYS_INPUT->GetSystemMousePos().GetMouseDragVec();
    MonoWrapper::SetObjectFieldValue<float>(vec3, "X", myVec2.x);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Y", myVec2.y);
    float myZ = 0.0f;
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Z", myZ);
    return vec3;
  }

  bool csToggleShowCursor()
  {
    return SYS_INPUT->GetSystemMousePos().ToggleCursorVisible();
  }

  void csSetCursorVisible(bool visible)
  {
    SYS_INPUT->GetSystemMousePos().SetCursorVisible(visible);
  }

  void csLoadCursorImage(const std::string& name)
  {
    SYS_INPUT->GetSystemMousePos().LoadCursorImage(name);
  }

  MonoObject* csGetClientRectSize()
  {
    MonoObject* vec3 = MonoWrapper::ConstructObject("Vector3");
    POINT myPoint = SYS_INPUT->GetSystemMousePos().GetClientRectSize();
    glm::vec2 myVec2;
    myVec2.x = static_cast<float>(myPoint.x);
    myVec2.y = static_cast<float>(myPoint.y);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "X", myVec2.x);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Y", myVec2.y);
    float myZ = 0.0f;
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Z", myZ);
    return vec3;
  }

  bool csToggleClipCursor()
  {
    return SYS_INPUT->GetSystemMousePos().ToggleClipCursor();
  }

  void csSetClipCursor(bool clip)
  {
    SYS_INPUT->GetSystemMousePos().SetClipCursor(clip);
  }

  MonoObject* csSetCursorPos(int x, int y)
  {
    MonoObject* vec3 = MonoWrapper::ConstructObject("Vector3");
    POINT myPoint = SYS_INPUT->GetSystemMousePos().SetCurPos(x, y);
    glm::vec2 myVec2;
    myVec2.x = static_cast<float>(myPoint.x);
    myVec2.y = static_cast<float>(myPoint.y);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "X", myVec2.x);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Y", myVec2.y);
    float myZ = 0.0f;
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Z", myZ);
    return vec3;
  }

  void csSetToCenter(bool set)
  {
    SYS_INPUT->GetSystemMousePos().SetToCenter(set);
  }

  MonoObject* csGetRelativeLocation()
  {
    MonoObject* vec3 = MonoWrapper::ConstructObject("Vector3");
    glm::vec2 myVec2 = SYS_INPUT->GetSystemMousePos().GetRelativeLocation();
    MonoWrapper::SetObjectFieldValue<float>(vec3, "X", myVec2.x);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Y", myVec2.y);
    float myZ = 0.0f;
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Z", myZ);
    return vec3;
  }

  MonoObject* csGetRelativeDragVec()
  {
    MonoObject* vec3 = MonoWrapper::ConstructObject("Vector3");
    glm::vec2 myVec2 = SYS_INPUT->GetSystemMousePos().GetRelativeDragVec();
    MonoWrapper::SetObjectFieldValue<float>(vec3, "X", myVec2.x);
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Y", myVec2.y);
    float myZ = 0.0f;
    MonoWrapper::SetObjectFieldValue<float>(vec3, "Z", myZ);
    return vec3;
  }

  void csSetScroll(short scroll)
  {
    SYS_INPUT->GetSystemMousePos().SetScroll(scroll);
  }

  bool csGetIfScrollUp()
  {
    return SYS_INPUT->GetSystemMousePos().GetIfScrollUp();
  }

  bool csGetIfScrollDown()
  {
    return SYS_INPUT->GetSystemMousePos().GetIfScrollDown();
  }

  void csResetWinSize()
  {
    SYS_INPUT->GetSystemMousePos().ResetWinSize();
  }
}