#pragma once
#include "LocalVector.h"  // replaces std::vector
#include "..\..\ISerializable.h"
#include <string>

#include <filesystem>

namespace fs = std::filesystem;

typedef class ComponentLoadAudio : public ISerializable
{
public:
  struct data
  {
    char path[512];
    char name[128];
  };
  // path, user defined name
  //using data = std::pair<const char*, const char*>;
  // Max sounds loaded.
  const static size_t MAX_SIZE = 24;
  // Always active
  // vector of sound paths
  LocalVector<data, MAX_SIZE> _sounds;

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