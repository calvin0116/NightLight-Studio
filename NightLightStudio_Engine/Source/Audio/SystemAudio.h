#pragma once
#include <map>
#include <utility>
#include <string>

#include "..\..\FMOD\inc\fmod.hpp"
#include "..\Math\Vector.h"
#include "..\Core\MySystem.h"
//#include "..\..\dep\FMOD\inc\fmod_errors.h"

// For messaging/event
#include "../Messaging/SystemReceiver.h"
#include "../Messaging/Messages/MessageTogglePlay.h"

class SystemAudio : public MySystem, public  Singleton<SystemAudio>
{
  // Singleton
  friend Singleton<SystemAudio>;
  // Alias
  using MyAudioMap = std::map<std::string, FMOD::Sound*>;

  // Max channels allowed
  const static int    s_MAX_CHANNELS = 32;
  // Units per meter for engine
  const static float  s_UNITS_PER_METER;

  // to store sound handles
  MyAudioMap          _sounds;
  // to store listener position, default at 0,0,0
  glm::vec3*          _listenerVecPos;
  FMOD_VECTOR         _listenerPos;
  // to convert positions
  FMOD_VECTOR         _fmodPos;
  // to store channel handles
  FMOD::Channel*      _channels[s_MAX_CHANNELS]{nullptr};
  int                 _objIDs[s_MAX_CHANNELS]{0};
  // FMOD system
  FMOD::System*       _system;

  // FMOD channels
  FMOD::ChannelGroup* _master;
  FMOD::ChannelGroup* _bgm;
  FMOD::ChannelGroup* _sfx;

  void  LoadSound(const std::string& _soundPath, const std::string& _name);
  void  ReleaseSounds();
  // Overloaded function for testing only! Do not use this function!
  void  Play3DOnce(const std::string& name, float x, float y, float z);


  // For receiving event/message
  SystemMessaging::SystemReceiver r;
  static bool _isPlaying;
  bool _Inited = false;
public: // Suppose to be private and use messaging, but since SystemAudio is singleton might as well make it public.
  // Audio System functions
  void  Pause(const int _channelID);
  void  Unpause(const int _channelID);
  void  Stop(const int _channelID);
  int   PlayBGM(const std::string& _name);
  void  PlayOnce(const std::string& _name);
  // Object ID = entity ID; to get transform position to follow for audio.
  int   Play3DLoop(const std::string& _name, const int _objID);
  // Object ID = entity ID; to get transform position to follow for audio.
  void  Play3DOnce(const std::string& name, const  int _objID);
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
  SystemAudio() : _listenerVecPos(nullptr), _listenerPos({ 0.0f, 0.0f, 0.0f }), _fmodPos({ 0.0f, 0.0f, 0.0f }), _system(nullptr), _master(nullptr), _bgm(nullptr), _sfx(nullptr) {}
  // no copy ctor
  SystemAudio(const SystemAudio&) = delete;
  // no copy assign
  SystemAudio& operator=(const SystemAudio&) = delete;
  // dtor
  ~SystemAudio() = default;

  // System functions
  void Load() override;
// void EarlyInit() override {};
  void Init() override;
  //void LateInit() override {};

  // Not used
  void GameLoad() override;
  void GameInit() override;
  void GameExit() override;

  void MyGameInit();

  void FixedUpdate(float dt) override { dt; };
  void Update() override;

	void Free();
  void Exit() override;

  void HandleTogglePlay(MessageTogglePlay&);
};

// Global ptr
static SystemAudio* SYS_AUDIO = SystemAudio::GetInstance();