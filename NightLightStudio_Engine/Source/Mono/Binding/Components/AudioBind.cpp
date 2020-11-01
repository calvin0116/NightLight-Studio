#include "AudioBind.h"
#include "..\..\MonoWrapper.h" // BindClassFunction
#include "..\..\..\Audio\SystemAudio.h" // C++ source function


namespace AudioBind
{
  void BindAudio()
  {
    MonoWrapper::BindClassFunction(csPlayBGM, "PlayBGM",
      "Audio");

    MonoWrapper::BindClassFunction(csPlayOnce, "PlayOnce",
      "Audio");

    MonoWrapper::BindClassFunction(csPlay3DLoop, "Play3DLoop",
      "Audio");

    MonoWrapper::BindClassFunction(csPlay3DOnce, "Play3DOnce",
      "Audio");

    MonoWrapper::BindClassFunction(csPause, "Pause",
      "Audio");

    MonoWrapper::BindClassFunction(csUnpause, "Unpause",
      "Audio");

    MonoWrapper::BindClassFunction(csStop, "Stop",
      "Audio");

    MonoWrapper::BindClassFunction(csMuteBGM, "MuteBGM",
      "Audio");

    MonoWrapper::BindClassFunction(csMuteSFX, "MuteSFX",
      "Audio");

    MonoWrapper::BindClassFunction(csMuteMASTER, "MuteMASTER",
      "Audio");

    MonoWrapper::BindClassFunction(csSetBGMVol, "SetBGMVol",
      "Audio");

    MonoWrapper::BindClassFunction(csSetSFXVol, "SetSFXVol",
      "Audio");

    MonoWrapper::BindClassFunction(csSetMASTERVol, "SetMASTERVol",
      "Audio");

    MonoWrapper::BindClassFunction(csMASTERPause, "PauseMASTER",
      "Audio");
  }

  int csPlayBGM(MonoString* _name)
  {
    return SYS_AUDIO->PlayBGM(MonoWrapper::ToString(_name));
  }
           
  void csPlayOnce(MonoString* _name)
  {
    SYS_AUDIO->PlayOnce(MonoWrapper::ToString(_name));
  }

  int csPlay3DLoop(MonoString* _name, int _entity)
  {
    return SYS_AUDIO->Play3DLoop(MonoWrapper::ToString(_name), _entity);
  }
           
  void csPlay3DOnce(MonoString* _name, int _entity)
  {
    SYS_AUDIO->Play3DOnce(MonoWrapper::ToString(_name), _entity);
  }

  void csPause(int _channel)
  {
    SYS_AUDIO->Pause(_channel);
  }

  void csUnpause(int _channel)
  {
    SYS_AUDIO->Unpause(_channel);
  }
           
  void csStop(int _channel)
  {
    SYS_AUDIO->Stop(_channel);
  }

  void csMuteBGM(bool _mute)
  {
    SYS_AUDIO->MuteBGM(_mute);
  }

  void csMuteSFX(bool _mute)
  {
    SYS_AUDIO->MuteSFX(_mute);
  }

  void csMuteMASTER(bool _mute)
  {
    SYS_AUDIO->MuteMASTER(_mute);
  }

  void csSetBGMVol(float _vol)
  {
    SYS_AUDIO->SetBGMVol(_vol);
  }

  void csSetSFXVol(float _vol)
  {
    SYS_AUDIO->SetSFXVol(_vol);
  }

  void csSetMASTERVol(float _vol)
  {
    SYS_AUDIO->SetMASTERVol(_vol);
  }

  void csMASTERPause(bool _pause)
  {
    SYS_AUDIO->PauseAll(_pause);
  }
}