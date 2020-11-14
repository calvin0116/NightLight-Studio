using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Graphics
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_isActive_Internal(this.native_handle); }
      set { set_isActive_Internal(this.native_handle, value); }
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
  }
}

