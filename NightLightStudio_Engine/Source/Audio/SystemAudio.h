#pragma once
#include <map>
#include <utility>
#include <string>

#include "..\..\dep\FMOD\inc\fmod.hpp"
#include "..\Math\Vector.h"
#include "..\Core\MySystem.h"
//#include "..\..\dep\FMOD\inc\fmod_errors.h"

class SystemAudio : public MySystem
{
  using MyAudioMap = std::map<std::string, FMOD::Sound*>;

  // Max channels allowed
  const static int    s_MAX_CHANNELS = 32;
  // Units per meter for engine
  const static float  s_UNITS_PER_METER;

  // to store sound handles
  MyAudioMap          _sounds;
  // to store listener position
  FMOD_VECTOR         _listenerPos;
  // to store channel handles
  FMOD::Channel*      _channels[s_MAX_CHANNELS]{nullptr};
  // FMOD system
  FMOD::System*       _system;

  // FMOD channels
  FMOD::ChannelGroup* _master;
  FMOD::ChannelGroup* _bgm;
  FMOD::ChannelGroup* _sfx;

  // Audio System functions
  void  LoadSound(const std::string& _soundPath, const std::string& _name);
  void  Pause(const int _channelID);
  void  Unpause(const int _channelID);
  void  Stop(const int _channelID);
  int   PlayBGM(const std::string& _name);
  void  PlayOnce(const std::string& _name);
  // Accepting position directly for now, need entity ID later on for checking active
  int   Play3DLoop(const std::string& _name, const NlMath::Vector3D& _pos);
  int   Play3DOnce(const std::string& name, const NlMath::Vector3D& _pos);
  // Inline
  inline void MuteBGM(bool _mute) { _bgm->setMute(_mute); }
  inline void MuteSFX(bool _mute) { _sfx->setMute(_mute); }
  inline void MuteMASTER(bool _mute) { _master->setMute(_mute); }
  inline void SetBGMVol(float _vol) { _bgm->setVolume(_vol); }
  inline void SetSFXVol(float _vol) { _sfx->setVolume(_vol); }
  inline void SetMASTERVol(float _vol) { _master->setVolume(_vol); }
  inline void PauseAll(bool _pause) { _master->setPaused(_pause); }
public:
  // ctor
  SystemAudio() : _listenerPos({ 0.0f, 0.0f, 0.0f }), _system(nullptr), _master(nullptr), _bgm(nullptr), _sfx(nullptr) {}
  // no copy ctor
  SystemAudio(const SystemAudio&) = delete;
  // no copy assign
  SystemAudio& operator=(const SystemAudio&) = delete;
  // dtor
  ~SystemAudio() = default;

  // System functions
	void OnFirstStart() override;

  void EarlyLoad() override {};
  void Load() override;
  void LateLoad() override {};

  void EarlyInit() override {};
  void Init() override;
  void LateInit() override {};

  bool FixedUpdate() override { return true; };
  bool Update() override;
  bool LateUpdate() override { return true; };

	void Exit();
	void Free();
	void Unload();
};