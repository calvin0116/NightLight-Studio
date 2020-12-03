using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Animation
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    void Play(string name, bool loop = true)
    {
      PlayAnimation_Internal(native_handle, name, loop);
    }

    void Pause()
    {
      PauseAnimation_Internal(native_handle);
    }

    void Resume()
    {
      ResumeAnimation_Internal(native_handle);
    }

    void Stop()
    {
      StopAnimation_Internal(native_handle);
    }

    bool IsFinished()
    {
      return IsFinished_Internal(native_handle);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PlayAnimation_Internal(IntPtr native_handle, string name, bool loop);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PauseAnimation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void ResumeAnimation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void StopAnimation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool IsFinished_Internal(IntPtr native_handle);
  }
}

