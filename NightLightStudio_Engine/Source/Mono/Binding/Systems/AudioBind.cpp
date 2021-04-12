#include "AudioBind.h"
#include "..\..\MonoWrapper.h" // BindClassFunction
#include "..\..\..\Audio\SystemAudio.h" // C++ source function


namespace AudioBind
{
  void BindAudio()
  {
    //MonoWrapper::BindClassFunction(csPlayBGM, "PlayBGM",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csPlayOnce, "PlayOnce",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csPlay3DLoop, "Play3DLoop",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csPlay3DOnce, "Play3DOnce",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csFadeOut, "FadeOut",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csPause, "Pause",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csUnpause, "Unpause",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csStop, "Stop",
    //  "Audio");

    //MonoWrapper::BindClassFunction(csSetVol, "SetVol",
    //  "Audio");

    MonoWrapper::BindClassFunction(csMuteBGM, "MuteBGM",
      "Audio");
    MonoWrapper::BindClassFunction(csMuteSFX, "MuteSFX",
      "Audio");
    MonoWrapper::BindClassFunction(csMuteUI, "MuteUI",
      "Audio");
    MonoWrapper::BindClassFunction(csMuteMASTER, "MuteMASTER",
      "Audio");
    MonoWrapper::BindClassFunction(csPauseBGM, "PauseBGM",
      "Audio");
    MonoWrapper::BindClassFunction(csPauseSFX, "PauseSFX",
      "Audio");
    MonoWrapper::BindClassFunction(csPauseUI, "PauseUI",
      "Audio");
    MonoWrapper::BindClassFunction(csPauseMASTER, "PauseMASTER",
      "Audio");
    MonoWrapper::BindClassFunction(csSetBGMVol, "VolumeBGM",
      "Audio");
    MonoWrapper::BindClassFunction(csSetSFXVol, "VolumeSFX",
      "Audio");
    MonoWrapper::BindClassFunction(csSetUIVol, "VolumeUI",
      "Audio");
    MonoWrapper::BindClassFunction(csSetMASTERVol, "VolumeMASTER",
      "Audio");

    MonoWrapper::BindClassFunction(csGetMuteBGM,"GetMuteBGM",
      "Audio");
    MonoWrapper::BindClassFunction(csGetMuteSFX, "GetMuteSFX",
      "Audio");
    MonoWrapper::BindClassFunction(csGetMuteMASTER, "GetMuteMASTER",
      "Audio");
    MonoWrapper::BindClassFunction(csGetPauseBGM, "GetPauseBGM",
      "Audio");
    MonoWrapper::BindClassFunction(csGetPauseSFX, "GetPauseSFX",
      "Audio");
    MonoWrapper::BindClassFunction(csGetPauseMASTER, "GetPauseMASTER",
      "Audio");
    MonoWrapper::BindClassFunction(csGetBGMVol, "GetVolumeBGM",
      "Audio");
    MonoWrapper::BindClassFunction(csGetSFXVol, "GetVolumeSFX",
      "Audio");
    MonoWrapper::BindClassFunction(csGetMASTERVol, "GetVolumeMASTER",
      "Audio");
  }

  //int csPlayBGM(MonoString* _name)
  //{
  //  return SYS_AUDIO->PlayBGM(MonoWrapper::ToString(_name));
  //}
  //         
  //int csPlayOnce(MonoString* _name)
  //{
  //  return SYS_AUDIO->PlayOnce(MonoWrapper::ToString(_name));
  //}

  //int csPlay3DLoop(MonoString* _name, int _entity)
  //{
  //  return SYS_AUDIO->Play3DLoop(MonoWrapper::ToString(_name), _entity);
  //}
  //         
  //int csPlay3DOnce(MonoString* _name, int _entity)
  //{
  //  return SYS_AUDIO->Play3DOnce(MonoWrapper::ToString(_name), _entity);
  //}

  //void csFadeOut(int _channel, float _duration)
  //{
  //  SYS_AUDIO->FadeOut(_channel, _duration);
  //}

  //void csPause(int _channel)
  //{
  //  SYS_AUDIO->Pause(_channel);
  //}

  //void csUnpause(int _channel)
  //{
  //  SYS_AUDIO->Unpause(_channel);
  //}
  //         
  //void csStop(int _channel)
  //{
  //  SYS_AUDIO->Stop(_channel);
  //}

  //void csSetVol(int _channel, float _vol)
  //{
  //  if (_vol > 100.0f)
  //    _vol = 100.0f;
  //  else if (_vol < 0.0f)
  //    _vol = 0.0f;
  //  SYS_AUDIO->SetVol(_channel, _vol);
  //}

  void csMuteBGM(bool _mute)
  {
    SYS_AUDIO->MuteBGM(_mute);
  }
  void csMuteSFX(bool _mute)
  {
    SYS_AUDIO->MuteSFX(_mute);
  }
  void csMuteUI(bool _mute)
  {
    SYS_AUDIO->MuteUI(_mute);
  }
  void csMuteMASTER(bool _mute)
  {
    SYS_AUDIO->MuteMASTER(_mute);
  }
  void csPauseBGM(bool _mute)
  {
    SYS_AUDIO->PauseBGM(_mute);
  }
  void csPauseSFX(bool _mute)
  {
    SYS_AUDIO->PauseSFX(_mute);
  }
  void csPauseUI(bool _pause)
  {
    SYS_AUDIO->PauseUI(_pause);
  }
  void csPauseMASTER(bool _mute)
  {
    SYS_AUDIO->PauseMASTER(_mute);
  }
  void csSetBGMVol(float _vol)
  {
    SYS_AUDIO->VolumeBGM(_vol);
  }
  void csSetSFXVol(float _vol)
  {
    SYS_AUDIO->VolumeSFX(_vol);
  }
  void csSetUIVol(float _vol)
  {
    SYS_AUDIO->VolumeUI(_vol);
  }
  void csSetMASTERVol(float _vol)
  {
    if (_vol > 100.0f)
      _vol = 100.0f;
    else if (_vol < 0.0f)
      _vol = 0.0f;
    SYS_AUDIO->VolumeMASTER(_vol);
  }

  // Getter
  bool csGetMuteBGM()
  {
    return SYS_AUDIO->GetMuteBGM();
  }
  bool csGetMuteSFX()
  {
    return SYS_AUDIO->GetMuteSFX();
  }
  bool csGetMuteMASTER()
  {
    return SYS_AUDIO->GetMuteMASTER();
  }
  bool csGetPauseBGM()
  {
    return SYS_AUDIO->GetPauseBGM();
  }
  bool csGetPauseSFX()
  {
    return SYS_AUDIO->GetPauseSFX();
  }
  bool csGetPauseMASTER()
  {
    return SYS_AUDIO->GetPauseMASTER();
  }
  float csGetBGMVol()
  {
    return SYS_AUDIO->GetVolumeBGM();
  }
  float csGetSFXVol()
  {
    return SYS_AUDIO->GetVolumeSFX();
  }
  float csGetMASTERVol()
  {
    return SYS_AUDIO->GetVolumeMASTER();
  }
}