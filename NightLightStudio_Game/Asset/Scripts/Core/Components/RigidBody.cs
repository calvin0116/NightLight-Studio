using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class RigidBody
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

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

    public Vector3 GetForce()
    {
      return get_Force_Internal(native_handle);
    }

    public void SetForce(Vector3 force)
    {
      set_Force_Internal(native_handle, force);
    }

    public Vector3 GetAccel()
    {
      return get_Accel_Internal(native_handle);
    }

    public void SetAccel(Vector3 accel)
    {
      set_Accel_Internal(native_handle, accel);
    }
    public Vector3 GetVel()
    {
      return get_Vel_Internal(native_handle);
    }

    public void SetVel(Vector3 vel)
    {
      set_Vel_Internal(native_handle, vel);
    }

    // Getter/Setter for isActive
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isActive_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isActive_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for force
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Force_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Force_Internal(IntPtr native_handle, Vector3 val);

    // Getter/Setter for acceleration
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Accel_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Accel_Internal(IntPtr native_handle, Vector3 val);

    // Getter/Setter for Velocity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Vel_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Vel_Internal(IntPtr native_handle, Vector3 val);

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

