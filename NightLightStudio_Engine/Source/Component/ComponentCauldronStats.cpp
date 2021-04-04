#include "ComponentCauldronStats.h"

#include "Components.h"
//#include "ComponentTransform.h"


ComponentCauldronStats::ComponentCauldronStats() 
	:talisman{}
	, collider{}
	, magnitude{0.0f}
	, direction{0.0f,0.0f,0.0f}
{
	strcpy_s(ser_name,"CauldronStatsComponent");
}

ComponentCauldronStats::~ComponentCauldronStats()
{
}

inline void ComponentCauldronStats::Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
{ 
	g_ecman;
	//std::string talisman;
	//std::string collider;
	//float magnitude;
	//NlMath::Vector3D direction;

	for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
	{
		if (itr->name == "talisman")
			talisman = itr->value.GetString();
		else if (itr->name == "collider")
			collider = itr->value.GetString();
		else if (itr->name == "magnitude")
			magnitude = itr->value.GetFloat();
		else if (itr->name == "direction")
		{
			auto arr = itr->value.GetArray();
			direction.x = arr[0].GetFloat();
			direction.y = arr[1].GetFloat();
			direction.z = arr[2].GetFloat();
		}
	}
}

inline Value ComponentCauldronStats::Write() 
{
	Value val(rapidjson::kObjectType);
	
	NS_SERIALISER::ChangeData(&val, "talisman", rapidjson::StringRef(talisman.c_str()));	//string
	NS_SERIALISER::ChangeData(&val, "collider", rapidjson::StringRef(collider.c_str()));	//string
	NS_SERIALISER::ChangeData(&val, "magnitude", magnitude);						//float

	Value dir(rapidjson::kArrayType);
	dir.PushBack(direction.x, global_alloc);
	dir.PushBack(direction.y, global_alloc);
	dir.PushBack(direction.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "direction", dir);		//Array

	return val;
}

