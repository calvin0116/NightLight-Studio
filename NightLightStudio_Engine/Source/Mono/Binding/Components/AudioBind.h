#pragma once
#include <mono/metadata/object.h> //MonoString*


namespace AudioBind
{
  void BindAudio();

  int  csPlayBGM(MonoString* _name);
  void csPlayOnce(MonoString* _name);
  int  csPlay3DLoop(MonoString* _name, int _entity);
  void csPlay3DOnce(MonoString* _name, int _entity);
  void csPause(int _channel);
  void csUnpause(int _channel);
  void csStop(int _channel);
  void csMuteBGM(bool _mute);
  void csMuteSFX(bool _mute);
  void csMuteMASTER(bool _mute);
  void csSetBGMVol(float _vol);
  void csSetSFXVol(float _vol);
  void csSetMASTERVol(float _vol);
  void csMASTERPause(bool _pause);
}