using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public static class Input
  {
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetKeyPress(VK val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetKeyHold(VK val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetKeyUp(VK val);
  }
}
