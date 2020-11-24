using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Variables
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    //// Vector3 custom types
    //private Vector3 position = new Vector3();
    //private Vector3 rotation = new Vector3();
    //private Vector3 scale = new Vector3();

    public int getInt(int index)
    {
      return get_Int_Internal(native_handle, index);
    }
    public void setInt(int val, int index)
    {
      set_Int_Internal(native_handle, val, index);
    }

    public string getString(int index)
    {
      return get_String_Internal(native_handle, index);
    }
    public void setString(string val, int index)
    {
      set_String_Internal(native_handle, val, index);
    }

    public float getFloat(int index)
    {
      return get_Float_Internal(native_handle, index);
    }
    public void setFloat(float val, int index)
    {
      set_Float_Internal(native_handle, val, index);
    }

    // Getter/Setter for Int
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_Int_Internal(IntPtr native_handle, int index);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Int_Internal(IntPtr native_handle, int val, int index);

    // Getter/Setter for String
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string get_String_Internal(IntPtr native_handle, int index);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_String_Internal(IntPtr native_handle, string val, int index);

    // Getter/Setter for Float
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Float_Internal(IntPtr native_handle, int index);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Float_Internal(IntPtr native_handle, float val, int index);
  }
}

