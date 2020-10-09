#pragma once
#include "LocalVector.h"  // replaces std::vector
#include "Components.h"   // inherit required functions

class ComponentAudio : public IComponent
{
  struct ACData
  {
    // Is the sound active
    bool        _isActive       = false;
    // Loop the sound
    bool        _loop           = false;
    // Play on scene start
    bool        _playOnStart    = false;
    // name given after loading all the sounds path from the resource manager (not implemented yet as of 9 Oct 2020)
    std::string _name           = "";
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
};