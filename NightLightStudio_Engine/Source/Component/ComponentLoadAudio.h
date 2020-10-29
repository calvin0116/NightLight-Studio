#pragma once
#include "LocalVector.h"  // replaces std::vector

typedef class ComponentLoadAudio //: public IComponent
{
public:
  // path, user defined name
  using data = std::pair <std::string, std::string>;
  // Max sounds loaded.
  const static size_t MAX_SIZE = 24;
  // Always active
  // vector of sound paths
  LocalVector<data, MAX_SIZE> _sounds;

  ComponentLoadAudio();
	~ComponentLoadAudio();

	//read and write function for initialization from saved files
	void Read();
	void Write();
} LoadAudioComponent;