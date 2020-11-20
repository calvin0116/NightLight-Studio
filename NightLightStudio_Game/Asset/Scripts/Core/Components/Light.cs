using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Light
  {
    public enum Type
    {
      DIRECTIONAL = 0,
      POINT,
      SPOT,
      INVALID_TYPE
    };

    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_isActive_Internal(this.native_handle); }
      set { set_isActive_Internal(this.native_handle, value); }
    }

    public float intensity
    {
      get { return get_Intensity_Internal(this.native_handle); }
      set { set_Intensity_Internal(this.native_handle, value); }
    }

    public float attenuation
    {
      get { return get_Attenuation_Internal(this.native_handle); }
      set { set_Attenuation_Internal(this.native_handle, value); }
    }

    public float inCutOff
    {
      get { return get_inCutOff_Internal(this.native_handle); }
      set { set_inCutOff_Internal(this.native_handle, value); }
    }

    public float outCutOff
    {
      get { return get_outCutOff_Internal(this.native_handle); }
      set { set_outCutOff_Internal(this.native_handle, value); }
    }

    public Vector3 GetAmbient()
    {
      return get_Ambient_Internal(native_handle);
    }

    public void SetAmbient(Vector3 val)
    {
      set_Ambient_Internal(native_handle, val);
    }

    public Vector3 GetDiffuse()
    {
      return get_Diffuse_Internal(native_handle);
    }

    public void SetDiffuse(Vector3 val)
    {
      set_Diffuse_Internal(native_handle, val);
    }

    public Vector3 GetSpecular()
    {
      return get_Specular_Internal(native_handle);
    }

    public void SetSpecular(Vector3 val)
    {
      set_Specular_Internal(native_handle, val);
    }

    // Getter/Setter for name
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isActive_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isActive_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for Intensity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Intensity_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Intensity_Internal(IntPtr native_handle, float val);

    // Getter/Setter for Intensity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Attenuation_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Attenuation_Internal(IntPtr native_handle, float val);

    // Getter/Setter for InnerCutoff
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_inCutOff_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_inCutOff_Internal(IntPtr native_handle, float val);

    // Getter/Setter for OuterCutoff
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_outCutOff_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_outCutOff_Internal(IntPtr native_handle, float val);
    
    // Getter/Setter for Ambient
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Ambient_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Ambient_Internal(IntPtr native_handle, Vector3 val);

    // Getter/Setter for Diffuse
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Diffuse_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Diffuse_Internal(IntPtr native_handle, Vector3 val);

    // Getter/Setter for Specular
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Specular_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Specular_Internal(IntPtr native_handle, Vector3 val);

  }
}

