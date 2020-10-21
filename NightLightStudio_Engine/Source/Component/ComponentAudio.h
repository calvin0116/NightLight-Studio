#pragma once
#include "LocalVector.h"  // replaces std::vector

typedef class ComponentAudio //: public IComponent
{
  struct ACData
  {
    // Is the sound active
    bool        _isActive       = false;
    // Play on scene start
    bool        _playOnStart    = false;
    // Loop the sound
    bool        _loop           = false;
    // Sound is 3D
    bool        _is3D           = false;
    // user defined name, must be unique
    std::string _name           = "";
    // path of sound
    std::string _path           = "";
    // channelID given by SystemAudio
    int         _channel        = -1;
  };

  const static size_t MAX_SIZE = 4;
public:
  using Data = ACData;
  // Is the component active
  bool                        _isActive;
  LocalVector<Data, MAX_SIZE> _sounds;

  ComponentAudio();
	~ComponentAudio();

	//read and write function for initialization from saved files
	void Read();
	void Write();
} AudioComponent;