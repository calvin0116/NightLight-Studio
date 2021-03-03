using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  enum RENDERTYPE
  {
    SOLID = 0,
    TEXTURED
  }

  public class Graphics
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_isActive_Internal(this.native_handle); }
      set { set_isActive_Internal(this.native_handle, value); }
    }

    public bool renderEmission
    {
      get { return get_renderEmission_Internal(this.native_handle); }
      set { set_renderEmission_Internal(this.native_handle, value); }
    }

    public Vector3 GetEmissiveColor()
    {
      return get_EmissiveColor_Internal(this.native_handle);
    }
    public void SetEmissiveColor(Vector3 val)
    {
      set_EmissiveColor_Internal(this.native_handle, val);
    }

    public float emissiveIntensity
    {
      get { return get_EmissiveIntensity_Internal(this.native_handle); }
      set { set_EmissiveIntensity_Internal(this.native_handle, value); }
    }

    public float alpha
    {
      get { return get_Alpha_Internal(this.native_handle); }
      set { set_Alpha_Internal(this.native_handle, value); }
    }

    public void AddModel(string file)
    {
      csAddModel(native_handle, file);
    }
    public void AddAlbedoTexture(string file)
    {
      csAddAlbedoTexture(native_handle, file);
    }
    public void AddNormalTexture(string file)
    {
      csAddNormalTexture(native_handle, file);
    }
    public void AddMetallicTexture(string file)
    {
      csAddMetallicTexture(native_handle, file);
    }
    public void AddRoughnessTexture(string file)
    {
      csAddRoughnessTexture(native_handle, file);
    }
    public void AddAOTexture(string file)
    {
      csAddAOTexture(native_handle, file);
    }
    public void AddSpecularTexture(string file)
    {
      csAddSpecularTexture(native_handle, file);
    }

    // Getter/Setter for isActive
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isActive_Internal(IntPtr native_handle);
    // Getter/Setter for isActive
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isActive_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for render emission
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_renderEmission_Internal(IntPtr native_handle);
    // Getter/Setter for render emission
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_renderEmission_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for emissive color
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_EmissiveColor_Internal(IntPtr native_handle);
    // Getter/Setter for emissive color
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_EmissiveColor_Internal(IntPtr native_handle, Vector3 val);

    // Getter/Setter for emission intensity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_EmissiveIntensity_Internal(IntPtr native_handle);
    // Getter/Setter for emission intensity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_EmissiveIntensity_Internal(IntPtr native_handle, float val);

    // Graphics Functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void csAddModel(IntPtr native_handle, string file);

    // Graphics Functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void csAddAlbedoTexture(IntPtr native_handle, string file);

    // Graphics Functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void csAddNormalTexture(IntPtr native_handle, string file);

    // Graphics Functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void csAddMetallicTexture(IntPtr native_handle, string file);

    // Graphics Functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void csAddRoughnessTexture(IntPtr native_handle, string file);

    // Graphics Functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void csAddAOTexture(IntPtr native_handle, string file);

    // Graphics Functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void csAddSpecularTexture(IntPtr native_handle, string file);

    // Getter/Setter for emission intensity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Alpha_Internal(IntPtr native_handle);
    // Getter/Setter for emission intensity
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Alpha_Internal(IntPtr native_handle, float val);
  }
}

