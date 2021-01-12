using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Navigator
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public float speed
    {
      get { return get_Speed_Internal(this.native_handle); }
      set { set_Speed_Internal(this.native_handle, value); }
    }

    public bool isFollowing
    {
      get { return get_IsFollowing_Internal(this.native_handle); }
      set { set_IsFollowing_Internal(this.native_handle, value); }
    }

    public bool isPaused
    {
      get { return get_IsPaused_Internal(this.native_handle); }
      set { set_IsPaused_Internal(this.native_handle, value); }
    }

    public int NavState
    {
      get { return get_NavState_Internal(this.native_handle); }
      set { set_NavState_Internal(this.native_handle, value); }
    }

    // Getter/Setter for speed
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Speed_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Speed_Internal(IntPtr native_handle, float val);

    // Getter/Setter for isFollowing
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsFollowing_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsFollowing_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for Float
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsPaused_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsPaused_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for Float
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_NavState_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_NavState_Internal(IntPtr native_handle, int val);
  }
}

