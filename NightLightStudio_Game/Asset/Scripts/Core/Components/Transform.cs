using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Transform
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    //// Vector3 custom types
    //private Vector3 position = new Vector3();
    //private Vector3 rotation = new Vector3();
    //private Vector3 scale = new Vector3();

    public string Name
    {
    get { return get_Name_Internal(this.native_handle); }
    set { set_Name_Internal(this.native_handle, value); }
    }

    public Vector3 getPosition()
    {
      return get_Position_Internal(this.native_handle);
    }

    public void setPosition(Vector3 vec)
    {
      set_Position_Internal(this.native_handle, vec);
    }

    public Vector3 getRotation()
    {
      return get_Rotation_Internal(this.native_handle);
    }

    public void setRotation(Vector3 vec)
    {
      set_Rotation_Internal(this.native_handle, vec);
    }

    public Vector3 getScale()
    {
      return get_Scale_Internal(this.native_handle);
    }

    public void setScale(Vector3 vec)
    {
      set_Scale_Internal(this.native_handle, vec);
    }

    //public Vector3 Position
    //{
    //  get { return position; }
    //  set
    //  {
    //    position.x = value.x;
    //    position.y = value.y;
    //    position.z = value.z;
    //  }
    //}

    //public Vector3 Rotation
    //{
    //  get { return rotation; }
    //  set
    //  {
    //    rotation.x = value.x;
    //    rotation.y = value.y;
    //    rotation.z = value.z;
    //  }
    //}

    //public Vector3 Scale
    //{
    //  get { return scale; }
    //  set
    //  {
    //    scale.x = value.x;
    //    scale.y = value.y;
    //    scale.z = value.z;
    //  }
    //}

    // Getter/Setter for name
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string get_Name_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Name_Internal(IntPtr native_handle, string val);

    // Getter/Setter for position
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Position_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Position_Internal(IntPtr native_handle, Vector3 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Rotation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Rotation_Internal(IntPtr native_handle, Vector3 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Scale_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Scale_Internal(IntPtr native_handle, Vector3 val);
  }
}

