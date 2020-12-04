using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
  public class UIElement
  {
    public IntPtr native_handle = IntPtr.Zero;

    public bool OnClick()
    {
      return OnClick_Internal(this.native_handle);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool OnClick_Internal(IntPtr native_handle);
  }
}
