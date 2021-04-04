#pragma once
#include "..\\Math\Vector.h"

#include "..\\..\ISerializable.h"
#include "LocalString.h"

typedef struct ComponentCauldronStats: public ISerializable //: public IComponent
{
	//bool isActive;
	LocalString<256> talisman;
	LocalString<256> collider;
	float magnitude;
	NlMath::Vector3D direction;
	
	ComponentCauldronStats();
	~ComponentCauldronStats();

	void	Read(Value& val , NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override;
	Value	Write();
	Value& Write(Value& val) { return val; };
	virtual ComponentCauldronStats* Clone()
	{
		ComponentCauldronStats* newcomp = new ComponentCauldronStats();
		*newcomp = *this;
		return newcomp;
	}

} CauldronStatsComponent;