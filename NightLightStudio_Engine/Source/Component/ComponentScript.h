#pragma once
#include "LocalVector.h"  // replaces std::vector
#include "..\..\ISerializable.h"
#include <mono/metadata/object.h>

// Only BGM will use this the most.3D sound will follow this component's ObjID pos by default.
typedef class ComponentScript : public ISerializable
{
public:
  struct data
  {
    MonoObject* _pInstance;
    uint32_t    _GCHandle;
    bool        _isActive;        // Show on editor, can edit
    bool        _isRunning;       // Show on editor, can edit
    char        _ScriptName[128]; // Show on editor, can edit
  };
  bool  _isActive;                // Show on editor, can edit
  LocalVector<data, 10> _Scripts; // Show on editor, can edit

  ComponentScript();
	~ComponentScript();

	//read and write function for initialization from saved files
  void	Read(Value&) override { };
  Value	Write() override { return Value(); };
  Value& Write(Value& val) override { return val; };
} ScriptComponent;