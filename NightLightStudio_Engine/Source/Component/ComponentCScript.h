#pragma once
//#include "LocalVector.h"  // replaces std::vector
#include "LocalString.h"
#include "..\..\ISerializable.h"
#include "..\Logic\CScripts\IScript.h"

namespace ScriptType
{
  
}

// Only BGM will use this the most.3D sound will follow this component's ObjID pos by default.
typedef class ComponentCScript : public ISerializable
{
  //static int _blkCtrl;
public:
  bool            _isActive;  // Expose, can edit
  LocalString<20> _sName;     // Expose, can edit
  IScript*        _pScript;   // Don't touch
  int             _iTag;      // Expose, can edit. must be unique

  ComponentCScript();
	~ComponentCScript();

	//read and write function for initialization from saved files
  void	Read(Value&) override;
  Value	Write() override;
  Value& Write(Value& val) override { return val; }
  virtual ComponentCScript* Clone()
  {
    ComponentCScript* newcomp = new ComponentCScript();
    *newcomp = *this;
    return newcomp;
  }
} CScriptComponent;