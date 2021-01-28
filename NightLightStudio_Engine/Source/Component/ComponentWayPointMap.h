#pragma once
#include "../Ai/WayPointManager.h"
#include "..\\..\ISerializable.h"
#include "ComponentWayPoint.h"

//Component where it stores the list of way point and produce an map of paths

typedef class ComponentWayPointMap : public ISerializable //: public IComponent
{
	LocalVector<ComponentWayPoint*> cur_path;

	float rad_for_detect = 25.0f;		//Default radius detection
public:
	LocalVector<LocalString<125>> way_point_list;	//Standard way point using entity to plot

	ComponentWayPointMap()
	{
		strcpy_s(ser_name, "WayPointMapComponent");
	}

	void ClearCurPath()
	{
		cur_path.clear();
	}

	LocalVector<WayPointComponent*> GetPath()
	{
		return cur_path;
	}

	//Converts list of entity name to real entity for way point traversing
	void InitPath()
	{
		ClearCurPath();
		for (LocalString<125> & wp_str : way_point_list)
		{
			if (wp_str.empty())
				continue;
			Entity ent = G_ECMANAGER->getEntityUsingEntName(wp_str);
			cur_path.push_back(ent.getComponent<WayPointComponent>());
		}
	}

	int WPSize()
	{
		return (int)way_point_list.size();
	}


	virtual void	Read(Value& val)
	{
		for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
		{
			if (itr->name == "way_point_list")
			{
				auto string_list_val = itr->value.GetArray();

				if (way_point_list.size() == 0)
					for (unsigned i = 0; i < string_list_val.Size(); ++i)
						way_point_list.push_back(LocalString(string_list_val[i].GetString()));
				else
					for (unsigned i = 0; i < string_list_val.Size(); ++i)
						way_point_list.at(i) = LocalString(string_list_val[i].GetString());
			}
			if (itr->name == "radius_for_detection")
			{
				rad_for_detect = itr->value.GetFloat();
			}
		}
	}

	virtual Value	Write() {
		Value val(rapidjson::kObjectType);

		Value string_list_val(rapidjson::kArrayType);
		for (LocalString<125> & s : way_point_list)
			string_list_val.PushBack(rapidjson::StringRef(s.c_str()), global_alloc);
		NS_SERIALISER::ChangeData(&val, "way_point_list", string_list_val);
		NS_SERIALISER::ChangeData(&val, "radius_for_detection", rad_for_detect);
		return val;
	}

	virtual ComponentWayPointMap* Clone()
	{
		ComponentWayPointMap* newCWPM = new ComponentWayPointMap();
		*newCWPM = *this;
		return newCWPM;
	}

} WayPointMapComponent;
