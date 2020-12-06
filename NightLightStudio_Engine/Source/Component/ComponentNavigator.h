#pragma once
#include "..\\Math\Vector.h"
#include "..\\..\ISerializable.h"
//#include "../Ai/AiManager.h"
#include "../Ai/WayPointManager.h"
#include "../Messaging/Messages/MessageTogglePlay.h"
#include "../Messaging/SystemReceiver.h"

#include "../Core/DeltaTime.h"
#include <time.h>

enum WP_NAV_TYPE {
	WN_TOANDFRO = 0,
	WN_CIRCULAR,
	WN_RANDOM
};

typedef class ComponentNavigator : public ISerializable //: public IComponent
{
	//LocalVector<NS_AI::WayPoint*> way_point_list;
	//LocalVector<NS_AI::Edges*> edge_list;
	//LocalVector<WayPoint*> cur_path;		//Decided by Astar

	//Curent targetted waypoint

	//float startTime = 0.0f;


	//Toggle traversing nodes
	bool traverseFront = true;
public:
	bool isFollowing;
	bool isPaused;
	bool stopAtEachWayPoint;
	WP_NAV_TYPE wp_nav_type;

	float speed = 1.f;
	float curTime;
	float endTime = 0.0f;
	float rad_for_detect = 25.0f;		//Default radius detection

	LocalVector<LocalString<125>> way_point_list;	//Standard way point using entity to plot
	LocalVector<TransformComponent*> cur_path;
	
	int cur_wp_index;
	int prev_wp_index;

	ComponentNavigator()
		:isFollowing{ true }
		,isPaused{false}
		,cur_wp_index{ 0 }
		, stopAtEachWayPoint{false}
		, wp_nav_type{ WN_TOANDFRO }
		, curTime{-1.f}
	{
		strcpy_s(ser_name, "NavigatorComponent");
	}


	void CleanCurPath()
	{
		cur_path.clear();
		cur_wp_index = 0;
	}


	//Converts list of entity name to real entity for way point traversing
	void InitPath()
	{
		CleanCurPath();
		for (LocalString<125> & wp_str : way_point_list)
		{
			if (wp_str.empty())
				continue;
			Entity ent = G_ECMANAGER->getEntityUsingEntName(wp_str);
			cur_path.push_back(ent.getComponent<TransformComponent>());
		}
	}

	//=============== Getter / Setter =================//
	/*
	void SetCurrentPath(LocalVector<NS_AI::WayPoint*> wp_list)
	{
		CleanCurPath();
		//way_point_list = wp_list;
	}*/

	TransformComponent* GetCurWp()
	{
		return cur_path.at(cur_wp_index);
	}
	TransformComponent* GetPrevWp()
	{
		return cur_path.at(prev_wp_index);
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

			if (itr->name == "speed")
			{
				speed = itr->value.GetFloat();
			}
			if (itr->name == "stopAtEachWayPoint")
			{
				stopAtEachWayPoint = itr->value.GetBool();
			}			
			if (itr->name == "endTime")
			{
				endTime = itr->value.GetFloat();
			}
			if (itr->name == "radius_for_detection")
			{
				rad_for_detect = itr->value.GetFloat();
			}

		}
	};
	virtual Value	Write() { 
		Value val(rapidjson::kObjectType);

		Value string_list_val(rapidjson::kArrayType);
		for (LocalString<125>& s : way_point_list)
			string_list_val.PushBack(rapidjson::StringRef(s.c_str()), global_alloc);
		NS_SERIALISER::ChangeData(&val, "way_point_list", string_list_val);
		NS_SERIALISER::ChangeData(&val, "stopAtEachWayPoint", stopAtEachWayPoint);
		NS_SERIALISER::ChangeData(&val, "endTime", endTime);
		NS_SERIALISER::ChangeData(&val, "radius_for_detection", rad_for_detect);

		NS_SERIALISER::ChangeData(&val, "speed", speed);
		return val;
	};
	virtual Value& Write(Value& val) { return val; };	
	
	virtual ComponentNavigator* Clone() {
		ComponentNavigator* newcomp = new ComponentNavigator();
		*newcomp = *this;
		return newcomp;
	}
	//================ Getter / Setter ========================//

	//Function to set next way point to go to
	void SetNextWp()
	{
		prev_wp_index = cur_wp_index;
		if (stopAtEachWayPoint)
		{
			isPaused = true;
			curTime = 0.0f;
		}

		if (wp_nav_type == WN_RANDOM)
		{
			srand((unsigned int)time(NULL));
			cur_wp_index = rand() % cur_path.size();
		}
		else
		if (traverseFront)
			if (cur_wp_index < cur_path.size() - 1)
				++cur_wp_index;
			else {
				if (wp_nav_type == WN_TOANDFRO)
				{
					traverseFront = false;
					--cur_wp_index;
				}
				else if (wp_nav_type == WN_CIRCULAR)
				{
					cur_wp_index = 0;
				}
			}
		else
			if (cur_wp_index > 0)
				--cur_wp_index;
			else
			{
				traverseFront = true;
				++cur_wp_index;
			}
		
		//cur_path.at(prev_wp_index) = cur_path.at(cur_wp_index)
		//for(int)
		//NlMath::RayToAABB()
	}

	void SetSpeed(float spd)
	{
		speed = spd;
	}

	void SetIsFollowing(bool fol)
	{
		//if (fol && !nav_comp->isFollowing) //if going from false to true
			//FindClosestWP(nav_comp, )

		isFollowing = fol;
	}

	bool GetIsPaused()
	{
		return isPaused;
	}

	void SetIsPaused(bool pau)
	{
		isPaused = pau;
	}

} NavigatorComponent, NavComponent;