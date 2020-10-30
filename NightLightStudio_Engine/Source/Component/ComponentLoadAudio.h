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
  void	Read(Value& val) override{ 
    auto sound_array = val["ListOfSound"].GetArray();
    for (int i = 0; i < sound_array.Size(); ++i)
    {
      fs::path cur_path_name = sound_array[i].GetString();
      data MyData;
      std::string MyName = cur_path_name.stem().string();
      std::string MyPath = cur_path_name.parent_path().string() + "/" + MyName + cur_path_name.extension().string();
      strcpy_s(MyData.name, 128, MyName.c_str());
      strcpy_s(MyData.path, 512, MyPath.c_str());
      _sounds.push_back(MyData);
    }
  };
  virtual Value	Write() { return Value(); };
  virtual Value& Write(Value& val) { return val; };
} LoadAudioComponent;