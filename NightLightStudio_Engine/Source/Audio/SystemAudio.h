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

#include "../IO/Json/Parser.h"
// ComponentLoadAudio::data
#include "../Component/ComponentLoadAudio.h"

#define s_MAX_CHANNELS 32
#define s_UNITS_PER_METER 100

class SystemAudio : public MySystem, public  Singleton<SystemAudio>
{
  // Singleton
  friend Singleton<SystemAudio>;
  // For storing loaded audio
  std::vector<FMOD::Sound*> Sounds; // index, loaded sounds
  // Game Loop
  static bool _isPlaying;
  static bool _isLoaded;
  static bool _isInited;

  // FMOD Variables
  // Sytem
  FMOD::System* _system;
  // Channel groups
  FMOD::ChannelGroup* _master;
  FMOD::ChannelGroup* _bgm;
  FMOD::ChannelGroup* _sfx;
  FMOD::ChannelGroup* _ui;

  SystemMessaging::SystemReceiver r;

  //Json Audio List
  NS_SERIALISER::Parser AudioListParser;

public: 
	SystemAudio() :
		AudioListParser{ "AudioList", json_path }
	{}
  // Getting of audios' path
  std::vector<std::string> Audios; // index, path

  // System functions
  void Init() override;
  void Update() override;
  void Exit() override;

  void PlayOnAwake();
  void LoadAudios();

  // Play/Stop audio
  void Play(int entity, ComponentLoadAudio::data& MyData);
  void Stop(int entity, ComponentLoadAudio::data& MyData);
  // Inline
  inline void MuteSFX(bool _mute) { _sfx->setMute(_mute); }
  inline void MuteBGM(bool _mute) { _bgm->setMute(_mute); }
  inline void MuteUI(bool _mute) { _ui->setMute(_mute); }
  inline void MuteMASTER(bool _mute) { _master->setMute(_mute); }
  inline void PauseSFX(bool _pause) { _sfx->setPaused(_pause); }
  inline void PauseBGM(bool _pause) { _bgm->setPaused(_pause); }
  inline void PauseUI(bool _pause) { _ui->setPaused(_pause); }
  inline void PauseMASTER(bool _pause) { _master->setPaused(_pause); }
  inline void VolumeSFX(float _vol) { _sfx->setVolume(_vol); }
  inline void VolumeBGM(float _vol) { _bgm->setVolume(_vol); }
  inline void VolumeUI(float _vol) { _ui->setVolume(_vol); }
  inline void VolumeMASTER(float _vol) { _master->setVolume(_vol); }
  // Getter
  inline bool GetMuteSFX()
  {
    bool mute;
    _sfx->getMute(&mute);
    return mute;
  }
  inline bool GetMuteBGM()
  {
    bool mute;
    _bgm->getMute(&mute);
    return mute;
  }
  inline bool GetMuteMASTER()
  {
    bool mute;
    _master->getMute(&mute);
    return mute;
  }
  inline bool GetPauseSFX()
  {
    bool pause;
    _sfx->getPaused(&pause);
    return pause;
  }
  inline bool GetPauseBGM()
  {
    bool pause;
    _bgm->getPaused(&pause);
    return pause;
  }
  inline bool GetPauseMASTER()
  {
    bool pause;
    _master->getPaused(&pause);
    return pause;
  }
  inline float GetVolumeSFX()
  {
    float vol;
    _sfx->getVolume(&vol);
    return vol;
  }
  inline float GetVolumeBGM()
  {
    float vol;
    _bgm->getVolume(&vol);
    return vol;
  }
  inline float GetVolumeMASTER()
  {
    float vol;
    _master->getVolume(&vol);
    return vol;
  }
  // Called MyGameExit in game loop // workaround for botched game loop
  void GameExit() override;

  // Not used
  void Load() override {} // Not needed.
  void FixedUpdate(float dt) override { dt; } // Not used
  void GameLoad() override {}
  void GameInit() override {}
  void Free() override {} // Not needed

  // Maybe not needed
  void MyGameInit() {};
  void MyGameExit();

  // Save list
  void SaveList();
  // Load list 
  void LoadList();

//  // Alias
//  using MyAudioMap = std::map<std::string, FMOD::Sound*>;
//  using MyChnnlMap = std::map<int, float>;
//
//  // Max channels allowed
//  const static int    s_MAX_CHANNELS = 32;
//  // Units per meter for engine
//  const static float  s_UNITS_PER_METER;
//
//  // to store sound handles
//  MyAudioMap          _sounds;
//  MyChnnlMap          _channelIDs;
//  // to store listener position, default at 0,0,0
//  glm::vec3*          _listenerVecPos;
//  FMOD_VECTOR         _listenerPos;
//  // to convert positions
//  FMOD_VECTOR         _fmodPos;
//  // to store channel handles
//  FMOD::Channel*      _channels[s_MAX_CHANNELS]{nullptr};
//  int                 _objIDs[s_MAX_CHANNELS]{0};
//  // FMOD system
//  FMOD::System*       _system;
//
//  // FMOD channels
//  FMOD::ChannelGroup* _master;
//  FMOD::ChannelGroup* _bgm;
//  FMOD::ChannelGroup* _sfx;
//
//  void  LoadSound(const std::string& _soundPath, const std::string& _name);
//  void  ReleaseSounds();
//  // Overloaded function for testing only! Do not use this function!
//  //void  Play3DOnce(const std::string& name, float x, float y, float z);
//
//
//  // For receiving event/message
//  SystemMessaging::SystemReceiver r;
//  static bool _isPlaying;
//  bool _Inited = false;
//public: // Suppose to be private and use messaging, but since SystemAudio is singleton might as well make it public.
//  // Audio System functions
//  void  Pause(const int _channelID);
//  void  Unpause(const int _channelID);
//  void  Stop(const int _channelID);
//  void  SetVol(const int _channelID, float _vol);
//  int   PlayBGM(const std::string& _name);
//  int   PlayOnce(const std::string& _name);
//  // Object ID = entity ID; to get transform position to follow for audio.
//  int   Play3DLoop(const std::string& _name, const int _objID);
//  // Object ID = entity ID; to get transform position to follow for audio.
//  int   Play3DOnce(const std::string& name, const  int _objID);
//  void  FadeOut(const int _channelID, const float _seconds);
//  // Inline
//  inline void MuteBGM(bool _mute) { _bgm->setMute(_mute); }
//  inline void MuteSFX(bool _mute) { _sfx->setMute(_mute); }
//  inline void MuteMASTER(bool _mute) { _master->setMute(_mute); }
//  inline void SetBGMVol(float _vol) { _bgm->setVolume(_vol); }
//  inline void SetSFXVol(float _vol) { _sfx->setVolume(_vol); }
//  inline void SetMASTERVol(float _vol) { _master->setVolume(_vol); }
//  inline void PauseAll(bool _pause) { _master->setPaused(_pause); }
//public:
//  // ctor
//  SystemAudio() : _listenerVecPos(nullptr), _listenerPos({ 0.0f, 0.0f, 0.0f }), _fmodPos({ 0.0f, 0.0f, 0.0f }), _system(nullptr), _master(nullptr), _bgm(nullptr), _sfx(nullptr) {}
//  // no copy ctor
//  SystemAudio(const SystemAudio&) = delete;
//  // no copy assign
//  SystemAudio& operator=(const SystemAudio&) = delete;
//  // dtor
//  ~SystemAudio() = default;
//
//  // System functions
//  void Load() override;
//// void EarlyInit() override {};
//  void Init() override;
//  //void LateInit() override {};
//
//  // Not used
//  void GameLoad() override;
//  void GameInit() override;
//  void GameExit() override;
//
//  void MyGameInit();
//
//  void FixedUpdate(float dt) override { dt; };
//  void Update() override;
//
//	void Free();
//  void Exit() override;

  void HandleTogglePlay(MessageTogglePlay&);
};

// Global ptr
static SystemAudio* SYS_AUDIO = SystemAudio::GetInstance();