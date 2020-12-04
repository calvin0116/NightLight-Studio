using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Canvas
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public UIElement FindUI(uint index)
    {
      return FindUI_Internal(this.native_handle, index);
    }

    public UIElement FindUIByName(string name)
    {
      return FindUIByName_Internal(this.native_handle, name);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static UIElement FindUI_Internal(IntPtr native_handle, uint index);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static UIElement FindUIByName_Internal(IntPtr native_handle, string name);
  }
}

