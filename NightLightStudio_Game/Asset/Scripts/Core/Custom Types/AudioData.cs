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

    //public bool isBGM
    //{
    //  get { return get_Index_Internal(native_handle); }
    //  set { set_Index_Internal(native_handle, value); }
    //}

    //public bool isLoop
    //{
    //  get { return get_Index_Internal(native_handle); }
    //  set { set_Index_Internal(native_handle, value); }
    //}

    //public bool playOnAwake
    //{
    //  get { return get_Index_Internal(native_handle); }
    //  set { set_Index_Internal(native_handle, value); }
    //}

    //public float volume
    //{
    //  get { return get_Index_Internal(native_handle); }
    //  set { set_Index_Internal(native_handle, value); }
    //}

    //public bool is3D
    //{
    //  get { return get_Index_Internal(native_handle); }
    //  set { set_Index_Internal(native_handle, value); }
    //}

    //public float minDist
    //{
    //  get { return get_Index_Internal(native_handle); }
    //  set { set_Index_Internal(native_handle, value); }
    //}
    
    //public float maxDist
    //{
    //  get { return get_Index_Internal(native_handle); }
    //  set { set_Index_Internal(native_handle, value); }
    //}

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_Index_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Index_Internal(IntPtr native_handle, int val);

    //[MethodImpl(MethodImplOptions.InternalCall)]
    //public extern static bool get_IsBGM_Internal(IntPtr native_handle);
    //[MethodImpl(MethodImplOptions.InternalCall)]
    //public extern static void set_IsBGM_Internal(IntPtr native_handle, bool val);

    //[MethodImpl(MethodImplOptions.InternalCall)]
    //public extern static bool get_IsLoop_Internal(IntPtr native_handle);
    //[MethodImpl(MethodImplOptions.InternalCall)]
    //public extern static void set_IsLoop_Internal(IntPtr native_handle, bool val);

    //[MethodImpl(MethodImplOptions.InternalCall)]
    //public extern static bool get_PlayOnAwake_Internal(IntPtr native_handle);
    //[MethodImpl(MethodImplOptions.InternalCall)]
    //public extern static void set_PlayOnAwake_Internal(IntPtr native_handle, bool val);
  }
}
