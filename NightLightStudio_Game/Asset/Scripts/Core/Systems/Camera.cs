using System;
using System.Collections.Specialized;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public static class Camera
  {
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
  }
}
