#pragma once
//#include "LocalVector.h"  // replaces std::vector
#include "LocalString.h"  // LocalString
#include "..\..\ISerializable.h"
#include <mono/metadata/object.h>
#include <unordered_map>

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
  bool             _isActive;         // Show on editor, can edit
  bool             _isRunning;        // Show on editor, can edit
  LocalString<64>  _ScriptName;       // Show on editor, can edit
  data             _MonoData;         // Mono Data

  // For inspector of C# script
  //int              _savedCount;       // How many strings currently
  //LocalString<256> _SavedID[10];      // Saved IDs of string up to 10 max.
  LocalString<512> _SavedStrings[10]; // Saved values of string
  //LocalString<512> _SavedString[10]; // Saved values of string
  //LocalString<512> _SavedString1; // Saved values of string
  std::unordered_map<std::string, int> array_sizes;
  int arr_sz = 0;

  ComponentScript();
	~ComponentScript();

	//read and write function for initialization from saved files
	void	Read(Value&, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override;
  Value	Write() override;
  Value& Write(Value& val) override { return val; };
  virtual ComponentScript* Clone()
  {
	  ComponentScript* newcomp = new ComponentScript();
	  *newcomp = *this;
	  return newcomp;
  }
} ScriptComponent;