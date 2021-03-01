using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Canvas
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_IsActive_Internal(native_handle); }
      set { set_IsActive_Internal(native_handle, value); }
    }

    public void AddUI()
    {
      AddUI_Internal(this.native_handle);
    }

    public void RemoveUI(uint index)
    {
      RemoveUI_Internal(this.native_handle, index);
    }

    public void Sort()
    {
      SortUI_Internal(this.native_handle);
    }

    public UIElement FindUI(uint index)
    {
      return FindUI_Internal(this.native_handle, index);
    }

    public UIElement FindUI(string name)
    {
      return FindUIByName_Internal(this.native_handle, name);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsActive_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsActive_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void AddUI_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void RemoveUI_Internal(IntPtr native_handle, uint index);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SortUI_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static UIElement FindUI_Internal(IntPtr native_handle, uint index);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static UIElement FindUIByName_Internal(IntPtr native_handle, string name);
  }
}

