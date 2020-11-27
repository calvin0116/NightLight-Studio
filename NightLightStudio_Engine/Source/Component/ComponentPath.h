#pragma once
#include "..\\Math\Vector.h"
#include "..\\..\ISerializable.h"
//#include "../Ai/AiManager.h"
#include "../Ai/WayPointManager.h"

typedef class ComponentPath: public ISerializable //: public IComponent
{
	LocalVector<WayPoint*> way_point_list;
	LocalVector<Edges*> edge_list;
public:
	ComponentPath()
	{
		strcpy_s(ser_name, "PathComponent");
	}

	virtual void	Read(Value& val) 
	{
		
	};
	virtual Value	Write() { 
		Value val(rapidjson::kObjectType);
		/*
		Value wp_list_val(rapidjson::kArrayType);
		for (WayPoint* wp : way_point_list)
		{
			Value wp_val(rapidjson::kObjectType);
			wp_val.AddMember("Entity")

			wp_list_val.PushBack(wp_val, global_alloc);
		}
		NS_SERIALISER::ChangeData(&val, "WayPointList", wp_list_val);
		*/
		
		return val;
	};
	virtual Value& Write(Value& val) { return val; };	virtual ISerializable* Clone() { return new ISerializable(); }



} PathComponent;