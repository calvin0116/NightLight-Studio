#pragma once
#include "LocalVector.h"  // replaces std::vector

// Only BGM will use this the most.3D sound will follow this component's ObjID pos by default.
typedef class ComponentAudio //: public IComponent
{
  struct ACData
  {
    // Is the sound active
    bool        _isActive       = false;
    // Play on scene start, this should always be true as there is no other use for audio otherwise (Probably).
    bool        _playOnStart    = true;
    // Is looping
    bool        _loop           = false;
    // Sound is 3D
    bool        _is3D           = false;
    // Update sound's position, only used for 3D sounds
    bool        _updatePos      = false;
    // user defined name defined in load component
    std::string _name           = "";
    // channelID given by SystemAudio, used for bgms and loops only.
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