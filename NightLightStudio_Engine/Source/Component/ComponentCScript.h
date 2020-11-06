#pragma once
#include "LocalVector.h"  // replaces std::vector

// Only BGM will use this the most.3D sound will follow this component's ObjID pos by default.
typedef class ComponentCScript //: public IComponent
{
  struct ACData
  {
    
  };

  const static size_t MAX_SIZE = 4;
public:
  using Data = ACData;
  // Is the component active
  bool                        _isActive;
  LocalVector<Data, MAX_SIZE> _sounds;

  ComponentCScript();
	~ComponentCScript();

	//read and write function for initialization from saved files
	void Read();
	void Write();
} CScriptComponent;