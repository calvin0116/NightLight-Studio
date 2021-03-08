using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public static class Audio
  {
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int PlayBGM(string _name);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int PlayOnce(string _name);
    
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int Play3DLoop(string _name, int _entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int Play3DOnce(string _name, int _entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void FadeOut(int _channel, float _duration);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void Pause(int _channel);
    
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void Unpause(int _channel);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void Stop(int _channel);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetVol(int _channel, float _vol);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void MuteBGM(bool _mute);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void MuteSFX(bool _mute);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void MuteMASTER(bool _mute);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetBGMVol(float _vol);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetSFXVol(float _vol);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetMASTERVol(float _vol);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PauseMASTER(bool _pause);
  }
}
