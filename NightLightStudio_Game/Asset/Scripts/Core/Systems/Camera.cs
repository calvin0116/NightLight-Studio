using System;
using System.Collections.Specialized;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public static class Camera
  {
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 UpVector();
  }
}
