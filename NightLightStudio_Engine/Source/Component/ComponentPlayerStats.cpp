#include "ComponentPlayerStats.h"

#include "Components.h"
//#include "ComponentTransform.h"


constexpr float GRAVITY_DEF = 100.0f;

ComponentPlayerStats::ComponentPlayerStats() 
	:player_move_mag{1.0f}
	, player_fly_mag{1.0f}
	, player_max_energy{1}
	, player_possess_energy_drain{1}
	, player_moth_energy_drain{1}
	, camera_distance{1.0f}
	, camera_offset{1.0f,1.0f,1.0f}
	, player_max_speed{1.0}
{
	strcpy_s(ser_name,"PlayerStatsComponent");
}

ComponentPlayerStats::~ComponentPlayerStats()
{
}

inline void ComponentPlayerStats::Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
{ 
	g_ecman;
	//float player_move_mag;
	//float player_fly_mag;
	//int player_max_energy;
	//int player_possess_energy_drain;
	//int player_moth_energy_drain;
	
	//float camera_distance;
	//NlMath::Vector3D camera_offset;
	//float player_max_speed;

	for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
	{
		if (itr->name == "player_move_mag")
			player_move_mag = itr->value.GetFloat();
		else if (itr->name == "player_fly_mag")
			player_fly_mag = itr->value.GetFloat();
		else if (itr->name == "player_max_energy")
			player_max_energy = itr->value.GetInt();
		else if (itr->name == "player_possess_energy_drain")
			player_possess_energy_drain = itr->value.GetInt();
		else if (itr->name == "player_moth_energy_drain")
			player_moth_energy_drain = itr->value.GetInt();
		else if (itr->name == "camera_distance")
			camera_distance = itr->value.GetFloat();
		else if (itr->name == "camera_offset")
		{
			auto arr = itr->value.GetArray();
			camera_offset.x = arr[0].GetFloat();
			camera_offset.y = arr[1].GetFloat();
			camera_offset.z = arr[2].GetFloat();
		}
		else if(itr->name == "player_max_speed")
			player_max_speed = itr->value.GetFloat();
	}
}

inline Value ComponentPlayerStats::Write() 
{
	Value val(rapidjson::kObjectType);
	
	NS_SERIALISER::ChangeData(&val, "player_move_mag", player_move_mag);		//Bool
	NS_SERIALISER::ChangeData(&val, "player_fly_mag", player_fly_mag);	//Bool
	NS_SERIALISER::ChangeData(&val, "player_max_energy", player_max_energy);	//Float
	NS_SERIALISER::ChangeData(&val, "player_possess_energy_drain", player_possess_energy_drain);	//Float
	NS_SERIALISER::ChangeData(&val, "player_moth_energy_drain", player_moth_energy_drain);
	NS_SERIALISER::ChangeData(&val, "camera_distance", camera_distance);

	Value cam_offset(rapidjson::kArrayType);
	cam_offset.PushBack(camera_offset.x, global_alloc);
	cam_offset.PushBack(camera_offset.y, global_alloc);
	cam_offset.PushBack(camera_offset.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "camera_offset", cam_offset);		//Array
	
	NS_SERIALISER::ChangeData(&val, "player_max_speed", player_max_speed);

	return val;
}

