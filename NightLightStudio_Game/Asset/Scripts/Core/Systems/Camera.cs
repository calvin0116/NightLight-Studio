using System;
using System.Collections.Specialized;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public static class Camera
  {
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetPosition();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetUpVector();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetRightVector();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetViewVector();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetXZViewVector();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetXZViewVector_Back();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetXZViewVector_Left();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 GetXZViewVector_Right();
    // Camera functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetUseThirdPersonCam(bool set);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void ToggleUseThirdPersonCam();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetThirdPersonCamCanRotateAnot(bool set);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetThirdPersonCamCanZoomAnot(bool set);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetThirdPersonCamTarget(Vector3 tgt);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetThirdPersonCamDistance(float dist);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetThirdPersonCamPitchAndYaw(float pitch, float yaw);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float GetFOV();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetFOV(float fov);
  }
}
