using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Animation
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public void Play(string name, bool loop = true, double startFrame = -1.0, double endFrame = 1.0)
    {
      PlayAnimation_Internal(native_handle, name, loop, startFrame, endFrame);
    }

    public void Pause()
    {
      PauseAnimation_Internal(native_handle);
    }

    public void Resume()
    {
      ResumeAnimation_Internal(native_handle);
    }

    public void Stop()
    {
      StopAnimation_Internal(native_handle);
    }

    public bool isFinished()
    {
      return IsFinished_Empty_Internal(native_handle);
    }

    public bool IsFinished(string name)
    {
      return IsFinished_Internal(native_handle, name);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PlayAnimation_Internal(IntPtr native_handle, string name, bool loop, double startFrame, double endFrame);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PauseAnimation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void ResumeAnimation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void StopAnimation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool IsFinished_Empty_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool IsFinished_Internal(IntPtr native_handle, string name);
  }
}

