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
    bool ImGuiTab;
    // Standard variables
    int index;        // index of sound in audio imgui
    //bool isActive;    // Whether this sound is active
    bool isBGM;       // Whether sound is a BGM/Ambience or SFX
    bool isLoop;      // whether this sound is looping
    bool playOnAwake; // whether to play on isActive = true
    float volume;     // Volume of sound 0.0f - 1.0f
    bool is3D;        // Whether sound is 3D
    // 3D variables
    float minDist, maxDist;

    // no need to save channel variable! Don't touch it!
    // Hidden variables for system use
    FMOD::Channel* channel;    // Current channel sound is playing on if it is playing.
    AudioData() : ImGuiTab(true), index(-1), isBGM(false), isLoop(false), playOnAwake(false), volume(1.0f), is3D(false), minDist(0.5f), maxDist(100.0f), channel(nullptr) {}

    //void Read(Value& val)
    //{
    //  for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
    //  {
    //    if (itr->name == "userName")
    //    {
    //      userName = itr->value.GetString();
    //    }

    //  }
    //};
    //Value& Write(Value& val);
  };
public:
  using data = AudioData;
  LocalVector<data> MyAudios; // Save this

  ComponentLoadAudio();
	~ComponentLoadAudio();

  // Variables in C# side
  bool GetPlay(int _index);
  void Play(int _index);

  bool GetMute(int _index);
  void Mute(int _index, bool _mute);

  bool GetPause(int _index);
  void Pause(int _index, bool _pause);

  void SetLoop(int _index, bool _loop);
  void SetVolume(int _index, float _vol);
  void SetMinDist(int _index, float _vol);
  void SetMaxDist(int _index, float _vol);

	//read and write function for initialization from saved files
  virtual void	Read(Value& val) override;
  virtual Value	Write() override;
  virtual Value& Write(Value& val) override { return val; }
  virtual ComponentLoadAudio* Clone()
  {
	  ComponentLoadAudio* newcomp = new ComponentLoadAudio();
	  *newcomp = *this;
	  return newcomp;
  }
} LoadAudioComponent;