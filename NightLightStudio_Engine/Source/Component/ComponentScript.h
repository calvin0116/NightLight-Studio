#pragma once
//#include "LocalVector.h"  // replaces std::vector
#include "LocalString.h"  // LocalString
#include "..\..\ISerializable.h"
#include <mono/metadata/object.h>

// Only BGM will use this the most.3D sound will follow this component's ObjID pos by default.
typedef class ComponentScript : public ISerializable
{
public:
  struct data
  {
    bool            _Inited = false;
    MonoObject*     _pInstance = nullptr;
    uint32_t        _GCHandle  = 0;
  };
  bool            _isActive;    // Show on editor, can edit
  bool            _isRunning;   // Show on editor, can edit
  LocalString<64> _ScriptName;  // Show on editor, can edit
  data            _MonoData;    // Mono Data

  ComponentScript();
	~ComponentScript();

	//read and write function for initialization from saved files
	void	Read(Value&) override;
  Value	Write() override;
  Value& Write(Value& val) override { return val; };
  virtual ComponentScript* Clone()
  {
	  ComponentScript* newcomp = new ComponentScript();
	  *newcomp = *this;
	  return newcomp;
  }
} ScriptComponent;