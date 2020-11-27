#pragma once
#include "..\\Math\Vector.h"
#include "..\\..\ISerializable.h"
//#include "../Ai/AiManager.h"
#include "../Ai/WayPointManager.h"
#include "../Messaging/Messages/MessageTogglePlay.h"
#include "../Messaging/SystemReceiver.h"
typedef class ComponentNavigator : public ISerializable //: public IComponent
{
	//LocalVector<NS_AI::WayPoint*> way_point_list;
	//LocalVector<NS_AI::Edges*> edge_list;
	//LocalVector<WayPoint*> cur_path;		//Decided by Astar

						//Curent targetted waypoint
	int isFollowing;


public:
	LocalVector<LocalString<125>> way_point_list;	//Standard way point using entity to plot
	LocalVector<TransformComponent*> cur_path;
	int cur_wp_index;
	ComponentNavigator()
		:cur_wp_index{ 0 }
	{
		strcpy_s(ser_name, "NavigatorComponent");
	}


	void CleanCurPath()
	{
		cur_path.clear();
		cur_wp_index = 0;
	}

	void InitPath()
	{
		CleanCurPath();
		for (LocalString<125> & wp_str : way_point_list)
		{
			Entity ent = G_ECMANAGER->getEntityUsingEntName(wp_str);
			cur_path.push_back(ent.getComponent<TransformComponent>());
		}
	}

	//=============== Getter / Setter =================//
	void SetCurrentPath(LocalVector<NS_AI::WayPoint*> wp_list)
	{
		CleanCurPath();
		//way_point_list = wp_list;
	}

	TransformComponent* GetCurWp()
	{
		return cur_path.at(cur_wp_index);
	}

	virtual void	Read(Value& val) 
	{

		for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
		{
			if (itr->name == "way_point_list")
			{
			auto string_list_val = itr->value.GetArray();

			if (way_point_list.size() == 0)
				for (int i = 0; i < string_list_val.Size(); ++i)
					way_point_list.push_back(LocalString(string_list_val[i].GetString()));
			else
				for (int i = 0; i < string_list_val.Size(); ++i)
					way_point_list.at(i) = LocalString(string_list_val[i].GetString());
			}
		}
	};
	virtual Value	Write() { 
		Value val(rapidjson::kObjectType);

		Value string_list_val(rapidjson::kArrayType);
		for (LocalString<125>& s : way_point_list)
			string_list_val.PushBack(rapidjson::StringRef(s.c_str()), global_alloc);
		NS_SERIALISER::ChangeData(&val, "way_point_list", string_list_val);
		return val;
	};
	virtual Value& Write(Value& val) { return val; };	
	
	virtual ComponentNavigator* Clone() {
		ComponentNavigator* newcomp = new ComponentNavigator();
		*newcomp = *this;
		return newcomp;
	}
} NavigatorComponent, NavComponent;