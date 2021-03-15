using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Script
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_IsActive_Internal(native_handle); }
      set { set_IsActive_Internal(native_handle, value); }
    }

    // Getter/Setter for Int
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsActive_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsActive_Internal(IntPtr native_handle, bool val);
  }
}

