using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Collider
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isCollidable
    {
    get { return get_isCollidable_Internal(this.native_handle); }
    set { set_isCollidable_Internal(this.native_handle, value); }
    }

    public int tag
    {
      get { return get_colliderTag_Internal(this.native_handle); }
      set { set_colliderTag_Internal(this.native_handle, value); }
    }

    public Vector3 getCenter()
    {
      return get_Center_Internal(this.native_handle);
    }

    public void SetCenter(Vector3 vec)
    {
      set_Center_Internal(this.native_handle, vec);
    }

    public Vector3 GetExtend()
    {
      return get_Extend_Internal(this.native_handle);
    }

    public void SetExtend(Vector3 vec)
    {
      set_Extend_Internal(this.native_handle, vec);
    }

    public Vector3 GetRotation()
    {
      return get_Rotation_Internal(this.native_handle);
    }

    public void SetRotation(Vector3 vec)
    {
      set_Rotation_Internal(this.native_handle, vec);
    }

    // Getter/Setter for isCollidable
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isCollidable_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isCollidable_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for tag
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_colliderTag_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_colliderTag_Internal(IntPtr native_handle, int val);

    // Getter/Setter for collider box
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Center_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Center_Internal(IntPtr native_handle, Vector3 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Extend_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Extend_Internal(IntPtr native_handle, Vector3 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Rotation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Rotation_Internal(IntPtr native_handle, Vector3 val);
  }
}

