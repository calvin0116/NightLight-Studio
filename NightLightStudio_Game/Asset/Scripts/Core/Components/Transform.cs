using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Transform
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    // Vector3 custom types
    private Vector3 position = new Vector3();
    private Vector3 rotation = new Vector3();
    private Vector3 scale = new Vector3();

    public string Name
    {
    get { return get_Name_Internal(this.native_handle); }
    set { set_Name_Internal(this.native_handle, value); }
    }

    public Vector3 Position
    {
      get { return position; }
      set
      {
        position.x = value.x;
        position.y = value.y;
        position.z = value.z;
      }
    }

    public Vector3 Rotation
    {
      get { return rotation; }
      set
      {
        rotation.x = value.x;
        rotation.y = value.y;
        rotation.z = value.z;
      }
    }

    public Vector3 Scale
    {
      get { return scale; }
      set
      {
        scale.x = value.x;
        scale.y = value.y;
        scale.z = value.z;
      }
    }
    
    // Getter/Setter for name
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string get_Name_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Name_Internal(IntPtr native_handle, string val);
  }
}

