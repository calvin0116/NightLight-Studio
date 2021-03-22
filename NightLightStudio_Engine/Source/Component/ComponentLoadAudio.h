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
    // Standard variables
    int index;        // index of sound in audio imgui
    bool isActive;    // Whether this sound is active
    bool isBGM;       // Whether sound is a BGM/Ambience or SFX
    bool isLoop;      // whether this sound is looping
    bool playOnAwake; // whether to play on isActive = true
    float volume;     // Volume of sound 0.0f - 1.0f
    bool is3D;        // Whether sound is 3D
    // 3D variables
    float minDist, maxDist;
    // no need to save this variable!
    // Hidden variables for system use
    FMOD::Channel* channel;    // Current channel sound is playing on if it is playing.
    AudioData() : index(-1), isActive(false), isBGM(false), isLoop(false), playOnAwake(false), volume(1.0f), is3D(false), minDist(0.5f), maxDist(100.0f), channel(nullptr) {}

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
  //const size_t MySize = 10;
  LocalVector<data> MyAudios; // Save this
  //struct data
  //{
  //  char path[512];
  //  char name[128];
  //};
  //// path, user defined name
  ////using data = std::pair<const char*, const char*>;
  //// Max sounds loaded.
  //const static size_t MAX_SIZE = 24;
  //// Always active
  //// vector of sound paths
  //LocalVector<data, MAX_SIZE> _sounds;

  ComponentLoadAudio();
	~ComponentLoadAudio();

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