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
    public int tag
    {
    get { return get_Tag_Internal(native_handle); }
    set { set_Tag_Internal(native_handle, value); }
    }

    public string name
    {
    get { return get_Name_Internal(this.native_handle); }
    set { set_Name_Internal(this.native_handle, value); }
    }

    public Vector3 GetPosition()
    {
      return get_Position_Internal(this.native_handle);
    }

    public void SetPosition(Vector3 vec)
    {
      set_Position_Internal(this.native_handle, vec);
    }

    public Vector3 GetRotation()
    {
      return get_Rotation_Internal(this.native_handle);
    }

    public void SetRotation(Vector3 vec)
    {
      set_Rotation_Internal(this.native_handle, vec);
    }

    public Vector3 GetScale()
    {
      return get_Scale_Internal(this.native_handle);
    }

    public void SetScale(Vector3 vec)
    {
      set_Scale_Internal(this.native_handle, vec);
    }

    // Getter/Setter for tag
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_Tag_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Tag_Internal(IntPtr native_handle, int val);

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

