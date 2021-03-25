#pragma once
#include "..\\..\\ISerializable.h"
#include "..\Math\Vector.h"

// Only BGM will use this the most.3D sound will follow this component's ObjID pos by default.
typedef class ComponentListener : public ISerializable
{
public:
	bool _isActive;
	NlMath::Vector3D _front;

	ComponentListener();
	~ComponentListener();

	//read and write function for initialization from saved files
	void Read(Value& val);
	Value Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentListener* Clone()
	{
		ComponentListener* newcomp = new ComponentListener();
		//*newcomp = *this;
		return newcomp;
	}
} ListenerComponent;