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


    public int SetWpActive(int val, bool act)
    {
      return toggle_WayPointActive_Internal(native_handle, val, act);
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
    public bool IsAroundWP()
    {
      return IsAroundWP_Internal(native_handle);
    }
    public bool IsWithinCirclingRange()
    {
      return IsWithinCirclingRange_Internal(native_handle);
    }
	//Get current way point entity id 
    public int GetCurWpEntId()
    {
		return GetCurWpId_Internal(native_handle);
	}
	
	//Get active way point entity id using index in array 
	public int GetActiveWpId(int val)
    {
      return GetActiveWpId_Internal(native_handle, val);
    }
	//Get size of the way point list for the navigator -> to get all active waypoint, *from i in range[0 -> size] , call GetActiveWpId(i)*
    public int GetActiveWpIdListSize()
    {
      return GetActiveWpIdListSize_Internal(native_handle);
    }
	
	//Get active way point entity id using index in array 
	public int GetInactiveWpId(int val)
    {
      return GetInactiveWpId_Internal(native_handle, val);
    }
	//Get size of the way point list for the navigator -> to get all active waypoint, *from i in range[0 -> size] , call GetActiveWpId(i)*
    public int GetInactiveWpIdListSize()
    {
      return GetInactiveWpIdListSize_Internal(native_handle);
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
    public extern static int toggle_WayPointActive_Internal(IntPtr native_handle, int val, bool act);

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

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool IsAroundWP_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool IsWithinCirclingRange_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int GetCurWpId_Internal(IntPtr native_handle);
	
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int GetActiveWpId_Internal(IntPtr native_handle, int val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int GetActiveWpIdListSize_Internal(IntPtr native_handle);
	
	    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int GetInactiveWpId_Internal(IntPtr native_handle, int val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int GetInactiveWpIdListSize_Internal(IntPtr native_handle);

  }
}

