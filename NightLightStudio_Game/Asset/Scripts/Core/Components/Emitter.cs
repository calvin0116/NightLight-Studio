using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class Emitter
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_isActive_Internal(native_handle); }
      set { set_isActive_Internal(native_handle, value); }
    }

    public int id
    {
      get { return get_EmitterID_Internal(native_handle); }
      set { set_EmitterID_Internal(native_handle, value); }
    }

    public int shapeType
    {
      get { return get_ShapeType_Internal(native_handle); }
      set { set_ShapeType_Internal(native_handle, value); }
    }

    public float duration
    {
      get { return get_Duration_Internal(native_handle); }
      set { set_Duration_Internal(native_handle, value); }
    }

    public uint emitRate
    {
      get { return get_EmitRate_Internal(native_handle); }
      set { set_EmitRate_Internal(native_handle, value); }
    }

    public uint maxParticles
    {
      get { return get_MaxParticle_Internal(native_handle); }
      set { set_MaxParticle_Internal(native_handle, value); }
    }

    public float burstRate
    {
      get { return get_BurstRate_Internal(native_handle); }
      set { set_BurstRate_Internal(native_handle, value); }
    }

    public uint burstAmount
    {
      get { return get_BurstAmount_Internal(native_handle); }
      set { set_BurstAmount_Internal(native_handle, value); }
    }

    public float angle
    {
      get { return get_Angle_Internal(native_handle); }
      set { set_Angle_Internal(native_handle, value); }
    }

    public float radius
    {
      get { return get_Radius_Internal(native_handle); }
      set { set_Radius_Internal(native_handle, value); }
    }

    public bool randomizeSize
    {
      get { return get_RandomizeSize_Internal(native_handle); }
      set { set_RandomizeSize_Internal(native_handle, value); }
    }

    public float minParticleSize
    {
      get { return get_MinParticleSize_Internal(native_handle); }
      set { set_MinParticleSize_Internal(native_handle, value); }
    }

    public float maxParticleSize
    {
      get { return get_MaxParticleSize_Internal(native_handle); }
      set { set_MaxParticleSize_Internal(native_handle, value); }
    }

    public bool randomizeSpeed
    {
      get { return get_RandomizeSpeed_Internal(native_handle); }
      set { set_RandomizeSpeed_Internal(native_handle, value); }
    }

    public float minParticleSpeed
    {
      get { return get_MinParticleSpeed_Internal(native_handle); }
      set { set_MinParticleSpeed_Internal(native_handle, value); }
    }

    public float maxParticleSpeed
    {
      get { return get_MaxParticleSpeed_Internal(native_handle); }
      set { set_MaxParticleSpeed_Internal(native_handle, value); }
    }

    public bool randomizeLife
    {
      get { return get_RandomizeLife_Internal(native_handle); }
      set { set_RandomizeLife_Internal(native_handle, value); }
    }

    public float minParticleLife
    {
      get { return get_MinParticleLife_Internal(native_handle); }
      set { set_MinParticleLife_Internal(native_handle, value); }
    }

    public float maxParticleLife
    {
      get { return get_MaxParticleLife_Internal(native_handle); }
      set { set_MaxParticleLife_Internal(native_handle, value); }
    }

    public bool prewarm
    {
      get { return get_Prewarm_Internal(native_handle); }
      set { set_Prewarm_Internal(native_handle, value); }
    }

    public bool burst
    {
      get { return get_Burst_Internal(native_handle); }
      set { set_Burst_Internal(native_handle, value); }
    }

    public bool loop
    {
      get { return get_Loop_Internal(native_handle); }
      set { set_Loop_Internal(native_handle, value); }
    }

    public bool follow
    {
      get { return get_Follow_Internal(native_handle); }
      set { set_Follow_Internal(native_handle, value); }
    }

    public bool fade
    {
      get { return get_Fade_Internal(native_handle); }
      set { set_Fade_Internal(native_handle, value); }
    }

    public bool velocityOverTime
    {
      get { return get_VelocityOverTime_Internal(native_handle); }
      set { set_VelocityOverTime_Internal(native_handle, value); }
    }

    public bool sizeOverTime
    {
      get { return get_SizeOverTime_Internal(native_handle); }
      set { set_SizeOverTime_Internal(native_handle, value); }
    }

    public bool speedOverTime
    {
      get { return get_SpeedOverTime_Internal(native_handle); }
      set { set_SpeedOverTime_Internal(native_handle, value); }
    }

    public bool colourOverTime
    {
      get { return get_ColourOverTime_Internal(native_handle); }
      set { set_ColourOverTime_Internal(native_handle, value); }
    }

    public Vector4 colourA
    {
      get { return get_ColourA_Internal(native_handle); }
      set { set_ColourA_Internal(native_handle, value); }
    }

    public Vector4 colourB
    {
      get { return get_ColourB_Internal(native_handle); }
      set { set_ColourB_Internal(native_handle, value); }
    }

    public Vector4 colourStart
    {
      get { return get_ColourStart_Internal(native_handle); }
      set { set_ColourStart_Internal(native_handle, value); }
    }

    public Vector4 colourEnd
    {
      get { return get_ColourEnd_Internal(native_handle); }
      set { set_ColourEnd_Internal(native_handle, value); }
    }

    public Vector3 velocity
    {
      get { return get_Velocity_Internal(native_handle); }
      set { set_Velocity_Internal(native_handle, value); }
    }

    public void AddTexture(string val)
    {
      cs_AddTexture_Internal(this.native_handle, val);
    }

    public void Play()
    {
      cs_Play_Internal(this.native_handle);
    }

    public void Stop()
    {
      cs_Stop_Internal(this.native_handle);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_isActive_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_isActive_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_EmitterID_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_EmitterID_Internal(IntPtr native_handle, int val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_ShapeType_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_ShapeType_Internal(IntPtr native_handle, int val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Duration_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Duration_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static uint get_EmitRate_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_EmitRate_Internal(IntPtr native_handle, uint val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static uint get_MaxParticle_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MaxParticle_Internal(IntPtr native_handle, uint val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_BurstRate_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_BurstRate_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static uint get_BurstAmount_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_BurstAmount_Internal(IntPtr native_handle, uint val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Angle_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Angle_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_Radius_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Radius_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_RandomizeSize_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_RandomizeSize_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MinParticleSize_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MinParticleSize_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MaxParticleSize_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MaxParticleSize_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_RandomizeSpeed_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_RandomizeSpeed_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MinParticleSpeed_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MinParticleSpeed_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MaxParticleSpeed_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MaxParticleSpeed_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_RandomizeLife_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_RandomizeLife_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MinParticleLife_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MinParticleLife_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_MaxParticleLife_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_MaxParticleLife_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Prewarm_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Prewarm_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Burst_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Burst_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Loop_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Loop_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Follow_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Follow_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_Fade_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Fade_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_VelocityOverTime_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_VelocityOverTime_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_SizeOverTime_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_SizeOverTime_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_SpeedOverTime_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_SpeedOverTime_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_ColourOverTime_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_ColourOverTime_Internal(IntPtr native_handle, bool val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector4 get_ColourA_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_ColourA_Internal(IntPtr native_handle, Vector4 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector4 get_ColourB_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_ColourB_Internal(IntPtr native_handle, Vector4 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector4 get_ColourStart_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_ColourStart_Internal(IntPtr native_handle, Vector4 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector4 get_ColourEnd_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_ColourEnd_Internal(IntPtr native_handle, Vector4 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector3 get_Velocity_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Velocity_Internal(IntPtr native_handle, Vector3 val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void cs_AddTexture_Internal(IntPtr native_handle, string val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void cs_Play_Internal(IntPtr native_handle);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void cs_Stop_Internal(IntPtr native_handle);
  }
}

