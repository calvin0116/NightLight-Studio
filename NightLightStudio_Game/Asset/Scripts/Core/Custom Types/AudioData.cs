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

    public bool isBGM
    {
      get { return get_IsBGM_Internal(native_handle); }
      set { set_IsBGM_Internal(native_handle, value); }
    }

    public bool isLoop
    {
      get { return get_IsLoop_Internal(native_handle); }
      set { set_IsLoop_Internal(native_handle, value); }
    }

    public bool playOnAwake
    {
      get { return get_PlayOnAwake_Internal(native_handle); }
      set { set_PlayOnAwake_Internal(native_handle, value); }
    }

    public float volume
    {
      get { return get_Volume_Internal(native_handle); }
      set { set_Volume_Internal(native_handle, value); }
    }

    public bool is3D
    {
      get { return get_Is3D_Internal(native_handle); }
      set { set_Is3D_Internal(native_handle, value); }
    }

    public float minDist
    {
      get { return get_MinDist_Internal(native_handle); }
      set { set_MinDist_Internal(native_handle, value); }
    }

    public float maxDist
    {
      get { return get_MaxDist_Internal(native_handle); }
      set { set_MaxDist_Internal(native_handle, value); }
    }

    public bool mute
    {
      get { return get_IsMute_Internal(native_handle); }
      set { set_IsMute_Internal(native_handle, value); }
    }

    public bool pause
    {
      get { return get_IsPause_Internal(native_handle); }
      set { set_IsPause_Internal(native_handle, value); }
    }

    public void Play()
    {
      Play_Internal(native_handle);
    }

    public void Stop()
    {
      Stop_Internal(native_handle);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_Index_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Index_Internal(IntPtr native_handle, int val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsBGM_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsBGM_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsLoop_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsLoop_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_PlayOnAwake_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_PlayOnAwake_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Volume_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Volume_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Is3D_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Is3D_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MinDist_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MinDist_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MaxDist_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MaxDist_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsMute_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsMute_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsPause_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsPause_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void Play_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void Stop_Internal(IntPtr native_handle);
  }
}
