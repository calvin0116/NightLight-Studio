using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
  public class UIElement
  {
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_IsActive_Internal(this.native_handle); }
      set { set_IsActive_Internal(this.native_handle, value); }
    }

    public float width
    {
      get { return get_Width_Internal(this.native_handle); }
      set { set_Width_Internal(native_handle, value); }
    }

    public float height
    {
      get { return get_Height_Internal(this.native_handle); }
      set { set_Height_Internal(native_handle, value); }
    }

    public float alpha
    {
      get { return get_Alpha_Internal(this.native_handle); }
      set { set_Alpha_Internal(native_handle, value); }
    }

    public Vector3 GetPosition()
    {
      return get_Position_Internal(native_handle);
    }

    public void SetPosition(Vector3 val)
    {
      set_Position_Internal(native_handle, val);
    }

    public Vector3 GetColour()
    {
      return get_Colour_Internal(native_handle);
    }

    public void SetColour(Vector3 val)
    {
      set_Colour_Internal(native_handle, val);
    }

    public bool OnClick()
    {
      return OnClick_Internal(this.native_handle);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool OnClick_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsActive_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsActive_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Width_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Width_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Height_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Height_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Alpha_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Alpha_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Position_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Position_Internal(IntPtr native_handle, Vector3 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Colour_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Colour_Internal(IntPtr native_handle, Vector3 val);
  }
}
