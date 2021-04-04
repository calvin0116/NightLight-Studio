#pragma once
#include "LocalVector.h"  // replaces std::vector
#include "..\..\ISerializable.h"
#include <string>
// FMOD channel
#include "..\FMOD\inc\fmod.hpp"

//#include <filesystem>
//#define MySize 10 // Size of array, sounds allowed per component
//namespace fs = std::filesystem;

typedef class ComponentLoadAudio : public ISerializable
{
  struct AudioData
  {
    int objID = -1;
    bool ImGuiTab     = true;
    // Standard variables
    int index         = -1;        // index of sound in audio imgui
    //bool isActive;    // Whether this sound is active
    bool isBGM        = false;       // Whether sound is a BGM/Ambience or SFX
    bool isLoop       = false;      // whether this sound is looping
    bool playOnAwake  = false; // whether to play on isActive = true
    float volume      = 1.0f;     // Volume of sound 0.0f - 1.0f
    bool is3D         = false;        // Whether sound is 3D
    // 3D variables
    float minDist     = 0.5f;
    float maxDist     = 100.0f;
    
    // no need to save channel variable! Don't touch it!
    // Hidden variables for system use
    FMOD::Channel* channel = nullptr;

    AudioData() : ImGuiTab(true), index(-1), isBGM(false), isLoop(false), playOnAwake(false), volume(1.0f), is3D(false), minDist(0.5f), maxDist(100.0f) {}

    void Play();
    void Stop();
    void SetLoop(bool _loop);
    void SetVolume(float _vol);
    void Set3D(bool _3d);
    void SetMinDist(float _val);
    void SetMaxDist(float _val);
    bool GetMute() const;
    void SetMute(bool _mute);
    bool GetPause() const;
    void SetPause(bool _pause);
  };
public:
  using data = AudioData;
  LocalVector<data> MyAudios; // Save this

  ComponentLoadAudio();
	~ComponentLoadAudio();

  // Variables in C# side
  //bool GetPlay(int _index);
  //void Play(int _index);

  //bool GetMute(int _index);
  //void Mute(int _index, bool _mute);

  //bool GetPause(int _index);
  //void Pause(int _index, bool _pause);

  //void SetLoop(int _index, bool _loop);
  //void SetVolume(int _index, float _vol);
  //void SetMinDist(int _index, float _vol);
  //void SetMaxDist(int _index, float _vol);

	//read and write function for initialization from saved files
  virtual void	Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override;
  virtual Value	Write() override;
  virtual Value& Write(Value& val) override { return val; }
  virtual ComponentLoadAudio* Clone()
  {
	  ComponentLoadAudio* newcomp = new ComponentLoadAudio();
	  *newcomp = *this;
	  return newcomp;
  }
} LoadAudioComponent, AudioSourceComponent, ComponentAudioSource;