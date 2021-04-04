#pragma once
#include "..\\Math\Vector.h"

#include "..\\..\ISerializable.h"

typedef struct ComponentPlayerStats : public ISerializable //: public IComponent
{
	//bool isActive;
	float player_move_mag;
	float player_fly_mag;
	int player_max_energy;
	int player_possess_energy_drain;
	int player_moth_energy_drain;
	
	float camera_distance;
	NlMath::Vector3D camera_offset;
	float player_max_speed;

	ComponentPlayerStats();
	~ComponentPlayerStats();

	void	Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER)override;
	Value	Write();
	Value& Write(Value& val) { return val; };
	virtual ComponentPlayerStats* Clone()
	{
		ComponentPlayerStats* newcomp = new ComponentPlayerStats();
		*newcomp = *this;
		return newcomp;
	}

} PlayerStatsC,PlayerStatsComponent;