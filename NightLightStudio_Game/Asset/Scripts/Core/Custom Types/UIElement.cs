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

    public bool isAnimated
    {
      get { return get_IsAnimated_Internal(this.native_handle); }
      set { set_IsAnimated_Internal(this.native_handle, value); }
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

    public uint row
    {
      get { return get_Row_Internal(this.native_handle); }
      set { set_Row_Internal(native_handle, value); }
    }

    public uint column
    {
      get { return get_Column_Internal(this.native_handle); }
      set { set_Column_Internal(native_handle, value); }
    }

    public uint totalFrame
    {
      get { return get_TotalFrame_Internal(this.native_handle); }
      set { set_TotalFrame_Internal(native_handle, value); }
    }

    public uint framesPerSecond
    {
      get { return get_FramesPerSecond_Internal(this.native_handle); }
      set { set_FramesPerSecond_Internal(native_handle, value); }
    }

    public bool play
    {
      get { return get_Play_Internal(this.native_handle); }
      set { set_Play_Internal(native_handle, value); }
    }

    public bool loop
    {
      get { return get_Loop_Internal(this.native_handle); }
      set { set_Loop_Internal(native_handle, value); }
    }

    public void AddTexture(string fileName)
    {
      AddTexture_Internal(this.native_handle, fileName);
    }

    public void PlayAnimation(bool loop = false)
    {
      PlayAnimation_Internal(this.native_handle, loop);
    }

    public void StopAnimation()
    {
      StopAnimation_Internal(this.native_handle);
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

    public bool OnHover()
    {
      return OnHover_Internal(this.native_handle);
    }

    public bool OnEnter()
    {
      return OnEnter_Internal(this.native_handle);
    }

    public bool OnExit()
    {
      return OnExit_Internal(this.native_handle);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool OnClick_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool OnHover_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool OnEnter_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool OnExit_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsActive_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsActive_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsAnimated_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsAnimated_Internal(IntPtr native_handle, bool val);

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
    public extern static uint get_Row_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Row_Internal(IntPtr native_handle, uint val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static uint get_Column_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Column_Internal(IntPtr native_handle, uint val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static uint get_TotalFrame_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_TotalFrame_Internal(IntPtr native_handle, uint val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static uint get_FramesPerSecond_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_FramesPerSecond_Internal(IntPtr native_handle, uint val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Play_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Play_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Loop_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Loop_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void AddTexture_Internal(IntPtr native_handle, string fileName);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PlayAnimation_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void StopAnimation_Internal(IntPtr native_handle);

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
