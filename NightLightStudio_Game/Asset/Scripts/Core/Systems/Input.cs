using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public static class Input
  {
    // Keyboard
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetKeyPress(VK val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetKeyHold(VK val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetKeyUp(VK val);
    
    // Controller
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void Resetcontroller(bool allowController = false);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetCTRLButtonDown(uint button);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetButtonPress(uint button);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetButtonHold(uint button);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetButtonUp(uint button);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float GetTrigger(int lr);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetAnalog(int lr);

    // Mouse
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetMousePos();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetOriginalMousePos();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetMouseDragVec();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool ToggleShowCursor();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetCursorVisible(bool visible);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void LoadCursorImage(string name);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetClientRectSize();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool ToggleClipCursor();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetClipCursor(bool clip);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 SetCursorPos(int x, int y);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetToCenter(bool set);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetRelativeLocation();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetRelativeDragVec();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetScroll(short scroll);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetScrollUp();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetScrollDown();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void ResetWinSize();
  }
}
