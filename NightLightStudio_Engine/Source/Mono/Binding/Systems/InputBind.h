#pragma once
#include <mono/metadata/object.h> //MonoString*
#include "../../../Input/SI_KeyPress.h" // Virtual Key codes


namespace InputBind
{
  void BindInput();

  // Keyboard
  bool csGetKeyPress(SystemInput_ns::VirtualKey val);
  bool csGetKeyHold(SystemInput_ns::VirtualKey val);
  bool csGetKeyUp(SystemInput_ns::VirtualKey val);


  // Controller
  void csResetcontroller(bool allowController = false); // Resets all Controllers
  bool csCTRLButtonDown(unsigned int button);           // Checks directly if a button is down on the controller

  bool csGetButtonPress(unsigned int button);  // Checks if button is pressed
  bool csGetButtonHold(unsigned int button);   // Checks if button is held
  bool csGetButtonUp(unsigned int button);     // Checks if button is released

  float csGetTrigger(int lr);				// Left Trigger = 0, Right Trigger = 1, all other values default to Left
  MonoObject* csGetAnalog(int lr);			// Left Analog Stick = 0, Right Analog Stick = 1, all other values default to Left


  // Mouse
  MonoObject* csGetMousePos();          // Get Mouse Position - (0,0) Begins in Bottom-Left Corner
  MonoObject* csGetOriginalMousePos();  // Get Mouse Position - (0,0) Begins in Top-Left Corner
  MonoObject* csGetMouseDragVec();      // Get Mouse Movement vector - (0,0) Begins in Bottom-Left Corner
  
  bool csToggleShowCursor();                // Toggles if cursor is viewable or not
  void csSetCursorVisible(bool visible);    // Sets If can view cursor
  void csLoadCursorImage(const std::string& name); // Sets Cursor image - .cur or .ani files only
  MonoObject* csGetClientRectSize();        // Returns Client Rect Size in pixels
  bool csToggleClipCursor();                // Toggle to clip cursor
  void csSetClipCursor(bool clip);          // Set to clip cursor
  MonoObject* csSetCursorPos(int x, int y); // Set cursor position
  void csSetToCenter(bool set);
  MonoObject* csGetRelativeLocation();
  MonoObject* csGetRelativeDragVec();
  void csSetScroll(short scroll);
  bool csGetIfScrollUp();
  bool csGetIfScrollDown();
  void csResetWinSize();
}