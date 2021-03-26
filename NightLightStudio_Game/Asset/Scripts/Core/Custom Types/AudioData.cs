using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
  public class AudioData
  {
    public IntPtr native_handle = IntPtr.Zero;

    public int index
    {
      get { return get_Index_Internal(native_handle); }
      set { set_Index_Internal(native_handle, value); }
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_Index_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Index_Internal(IntPtr native_handle, int val);
  }
}
