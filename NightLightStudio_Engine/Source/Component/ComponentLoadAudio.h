#pragma once
#include "LocalVector.h"  // replaces std::vector
#include "..\..\ISerializable.h"
#include <string>

//#include <filesystem>
//#define MySize 10 // Size of array, sounds allowed per component
//namespace fs = std::filesystem;

typedef class ComponentLoadAudio : public ISerializable
{
  struct AudioData
  {
    // Standard variables
    std::string userName;
    bool isActive;
    bool isLoop;
    bool playOnAwake;
    bool is3D;
    // 3D variables
    float minDist, maxDist;
    AudioData() : userName(""), isActive(false), isLoop(false), playOnAwake(false), is3D(false) {}

    void Read(Value& val)
    {
      for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
      {
        if (itr->name == "userName")
        {
          userName = itr->value.GetString();
        }

      }
    };
    Value& Write(Value& val);
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