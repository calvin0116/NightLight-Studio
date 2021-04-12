#pragma once
#include <mono/metadata/object.h> //MonoString*


namespace AudioBind
{
  void BindAudio();

  //int csPlayBGM(MonoString* _name);
  //int csPlayOnce(MonoString* _name);
  //int csPlay3DLoop(MonoString* _name, int _entity);
  //int csPlay3DOnce(MonoString* _name, int _entity);
  //void csFadeOut(int _channel, float _duration);
  //void csPause(int _channel);
  //void csUnpause(int _channel);
  //void csStop(int _channel);
  //void csSetVol(int _channel, float _vol);
  void csMuteBGM(bool _mute);
  void csMuteSFX(bool _mute);
  void csMuteUI(bool _mute);
  void csMuteMASTER(bool _mute);
  void csPauseBGM(bool _pause);
  void csPauseSFX(bool _pause);
  void csPauseUI(bool _pause);
  void csPauseMASTER(bool _pause);
  void csSetBGMVol(float _vol);
  void csSetSFXVol(float _vol);
  void csSetUIVol(float _vol);
  void csSetMASTERVol(float _vol);
  // Getter
  bool csGetMuteBGM();
  bool csGetMuteSFX();
  bool csGetMuteMASTER();
  bool csGetPauseBGM();
  bool csGetPauseSFX();
  bool csGetPauseMASTER();
  float csGetBGMVol();
  float csGetSFXVol();
  float csGetMASTERVol();
}