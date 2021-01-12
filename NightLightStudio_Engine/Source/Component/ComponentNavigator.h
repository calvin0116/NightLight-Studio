#pragma once
#include "..\\Math\Vector.h"
#include "..\\..\ISerializable.h"
//#include "../Ai/AiManager.h"
#include "../Ai/WayPointManager.h"
#include "../Messaging/Messages/MessageTogglePlay.h"
#include "../Messaging/SystemReceiver.h"
#include "ComponentWayPointMap.h"

#include "../Core/DeltaTime.h"
#include "../Component/LocalString.h"
#include <time.h>

enum WP_NAV_TYPE {
	WN_TOANDFRO = 0,	// 1 -> N -> (Reset) -> 1 -> N
	WN_CIRCULAR,		// 1 -> N -> 1
	WN_RANDOM
};

enum WP_PATH_CREATION_TYPE {
	WPP_STANDARD = 0,	// 1 ->  N
	WPP_REVERSE,		// N -> 1
	WPP_CUSTOM,			// Inserted through script / leveleditor 
};

enum NAV_STATE {
	NV_PATROL = 0,		// Go from way point to way point
	NV_CIRCLING,		// Patrol around current way point
};

typedef class ComponentNavigator : public ISerializable //: public IComponent
{
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
	float size_in_rad = 25.0f;			//Circular detection to check if you hit the way point

	ComponentWayPointMap* cur_wp_path;	//Way points and edges for all routes
	LocalString<125> wp_path_ent_name;
	
	LocalVector<std::pair<int,bool>> path_indexes;		//Current following routes

	WP_PATH_CREATION_TYPE wp_creation_type;
 	int cur_wp_index;
	int prev_wp_index;
	
	NAV_STATE nav_state;
	float circuling_rad = 20.0f;

	ComponentNavigator()
		:isFollowing{ true }
		,isPaused{false}
		,cur_wp_index{ 0 }
		, stopAtEachWayPoint{false}
		, wp_nav_type{ WN_TOANDFRO }
		, curTime{-1.f}
		, cur_wp_path{nullptr}
		, wp_creation_type{WPP_STANDARD}
		, nav_state{ NV_CIRCLING }
		
	{
		strcpy_s(ser_name, "NavigatorComponent");
	}


	void CleanCurPath()
	{
		cur_wp_index = 0;
	}


	//=============== Getter / Setter =================//
	/*
	void SetCurrentPath(LocalVector<NS_AI::WayPoint*> wp_list)
	{
		CleanCurPath();
		//way_point_list = wp_list;
	}*/

	WayPointComponent* GetCurWp()
	{
		return cur_wp_path->GetPath().at(path_indexes.at(cur_wp_index).first);
	}
	WayPointComponent* GetPrevWp()
	{
		return cur_wp_path->GetPath().at(path_indexes.at(prev_wp_index).first);
	}

	//LocalVector<int>

	
	//Check if navigator have way point to follow
	bool HaveWayPoint()
	{
		return (cur_wp_path != nullptr && cur_wp_path->WPSize() > 1);
	}


	virtual void Read(Value& val) 
	{
		for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
		{
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
				size_in_rad = itr->value.GetFloat();
			}
			if (itr->name == "WayPointMapName")
			{
				//size_in_rad = itr->value.GetFloat();
				wp_path_ent_name = itr->value.GetString();
			}

		}
	};
	virtual Value	Write() { 
		Value val(rapidjson::kObjectType);

		NS_SERIALISER::ChangeData(&val, "stopAtEachWayPoint", stopAtEachWayPoint);
		NS_SERIALISER::ChangeData(&val, "endTime", endTime);

		NS_SERIALISER::ChangeData(&val, "speed", speed);
		NS_SERIALISER::ChangeData(&val, "radius_for_detection", size_in_rad);

		NS_SERIALISER::ChangeData(&val, "WayPointMapName", rapidjson::StringRef(wp_path_ent_name.c_str()));
		return val;
	};
	virtual Value& Write(Value& val) { return val; };	
	
	virtual ComponentNavigator* Clone() {
		ComponentNavigator* newcomp = new ComponentNavigator();
		*newcomp = *this;
		return newcomp;
	}
	//================ Getter / Setter ========================//
	void InitPath()
	{
		if (cur_wp_path == nullptr)
		{
			if (!wp_path_ent_name.empty())
				cur_wp_path = G_ECMANAGER->getEntityUsingEntName((std::string)wp_path_ent_name).getComponent<WayPointMapComponent>();
			else
				return;
		}

		cur_wp_index = 0;
		prev_wp_index = 0;

		switch (wp_creation_type) 
		{
		case WPP_STANDARD:
		{
			path_indexes.clear();
			int wp_size = cur_wp_path->GetPath().size();
			for (int i = 0; i < wp_size; ++i)
				path_indexes.push_back(std::make_pair(i, true));
			break;
		}
		case WPP_REVERSE:
		{
			path_indexes.clear();
			int wp_size = cur_wp_path->GetPath().size();
			for (int i = wp_size; i >= 0; ++i)
				path_indexes.push_back(std::make_pair(i, true));
			break;
		}
		case WPP_CUSTOM:	//Inserted beforehand
		{}

		}
	}

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
			cur_wp_index = rand() % cur_wp_path->GetPath().size();
		}
		else
		if (traverseFront)
			if (cur_wp_index < path_indexes.size() - 1)
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


		if (!path_indexes.at(cur_wp_index).second)
			SetNextWp();
		//2. Check for direct route

		//3. Find route to way point if being blocked
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

	void SetCurPathIndex(LocalVector<int> path)
	{
		for(int node : path)
			path_indexes.push_back(std::make_pair(node, true));
	}

	LocalVector<WayPointComponent*> GetCurPath()
	{
		return cur_wp_path->GetPath();
	}

	void TurnOffCurWayointPoint()
	{
		path_indexes.at(cur_wp_index).second = false;
	}

	void ToggleWayPointActive(int index, bool act)
	{
		path_indexes.at(index).second = act;
	}

} NavigatorComponent, NavComponent;