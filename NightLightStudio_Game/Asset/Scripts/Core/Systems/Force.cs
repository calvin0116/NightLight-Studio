using System;
using System.Collections.Specialized;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public static class Force
  {
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void Apply(int entity, Vector3 vec, float mag);
  }
}
