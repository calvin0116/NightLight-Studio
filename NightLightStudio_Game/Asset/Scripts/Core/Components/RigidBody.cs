using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class RigidBody
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    //// Vector3 custom types
    //private Vector3 position = new Vector3();
    //private Vector3 rotation = new Vector3();
    //private Vector3 scale = new Vector3();

    public bool isActive
    {
    get { return get_isActive_Internal(this.native_handle); }
    set { set_isActive_Internal(this.native_handle, value); }
    }

    public bool isStatic
    {
      get { return get_isStatic_Internal(this.native_handle); }
      set { set_isStatic_Internal(this.native_handle, value); }
    }

    public bool isGravity
    {
      get { return get_isGravity_Internal(this.native_handle); }
      set { set_isGravity_Internal(this.native_handle, value); }
    }

    public float mass
    {
      get { return get_Mass_Internal(this.native_handle); }
      set { set_Mass_Internal(this.native_handle, value); }
    }

    public float friction
    {
      get { return get_Friction_Internal(this.native_handle); }
      set { set_Friction_Internal(this.native_handle, value); }
    }

    // Getter/Setter for isActive
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isActive_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isActive_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for isStatic
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isStatic_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isStatic_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for isGravity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isGravity_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isGravity_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for Mass
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Mass_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Mass_Internal(IntPtr native_handle, float val);

    // Getter/Setter for Friction
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Friction_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Friction_Internal(IntPtr native_handle, float val);
  }
}

