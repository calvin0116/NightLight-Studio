#pragma once
#include "..\\..\ISerializable.h"
//#include "../Ai/WayPointManager.h"
#include "../Ai/AiGraph.h"

typedef class ComponentWayPoint: public ISerializable, public NS_AI::WayPoint
{
public:
	ComponentWayPoint()
	{
		strcpy_s(ser_name, "WayPointComponent");
	}
	
	virtual void	Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override
	{
		g_ecman;
		for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
		{
			if (itr->name == "SelfDefineVariable")
			{
				self_define_var.second = itr->value.GetFloat();
			}
		}
	}

	virtual Value	Write() {
		Value val(rapidjson::kObjectType);
		NS_SERIALISER::ChangeData(&val, "SelfDefineVariable", self_define_var.second);
		return val;
	}

	virtual ComponentWayPoint* Clone()
	{
		ComponentWayPoint* newCWP = new ComponentWayPoint();
		*newCWP = *this;
		return newCWP;
	}

	glm::vec3& GetPos() override;

} WayPointComponent;
