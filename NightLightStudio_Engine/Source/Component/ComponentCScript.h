#pragma once
#include "LocalVector.h"  // replaces std::vector

// Only BGM will use this the most.3D sound will follow this component's ObjID pos by default.
typedef class ComponentCScript //: public IComponent
{

public:
  bool _isActive;

  ComponentCScript();
	~ComponentCScript();

	//read and write function for initialization from saved files
	void Read();
	void Write();
} CScriptComponent;