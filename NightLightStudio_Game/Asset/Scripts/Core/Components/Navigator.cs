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

    public Vector3 GetDir()
    {
        return get_Dir_Internal(native_handle);
    }


    public void SetWpActive(int val, bool act)
    {
      toggle_WayPointActive_Internal(native_handle, val, act);
    }

    public bool MoreThenOneWPActive()
    {
      return MoreThenOneWPActive_Internal(native_handle);
    }

    public void GoToNextWp()
    {
       GoToNextWp_Internal(native_handle);
    }

    public float DistFromCurWp()
    {
      return getDistFromCurPathWP_Internal(native_handle);
    }
    public float DistFromPrevWp()
    {
      return getDistFromPrevPathWP_Internal(native_handle);
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

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void toggle_WayPointActive_Internal(IntPtr native_handle, int val, bool act);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool MoreThenOneWPActive_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GoToNextWp_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Dir_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float getDistFromCurPathWP_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float getDistFromPrevPathWP_Internal(IntPtr native_handle);
  }
}

