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
	WPP_STANDARD = 0,	// 1 -> N
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
	glm::vec3 dir = {0.0f,0.0f,0.0f};

	ComponentWayPointMap* cur_wp_path;	//Way points and edges for all routes
	ComponentTransform* trans;
	ComponentRigidBody* rb;

	LocalString<125> wp_path_ent_name;
	
	LocalVector<std::pair<int,bool>> path_indexes;		//Current following routes
	WP_PATH_CREATION_TYPE wp_creation_type;
 	int cur_path_wp_index = 0;
	int prev_path_wp_index = 0;

	LocalVector<int> wp_to_reach_end;					//List of way point to go from one point to another -> navigation
	int cur_route_wp_index = 0;
	int prev_route_wp_index = 0;
	
	NAV_STATE nav_state;
	float circuling_rad = 20.0f;

	ComponentNavigator()
		:isFollowing{ true }
		,isPaused{false}
		, cur_path_wp_index{ 0 }
		, stopAtEachWayPoint{false}
		, wp_nav_type{ WN_TOANDFRO }
		, curTime{-1.f}
		, cur_wp_path{nullptr}
		, wp_creation_type{WPP_STANDARD}
		, nav_state{ NV_CIRCLING }
		, wp_path_ent_name{""}
		
	{
		strcpy_s(ser_name, "NavigatorComponent");
	}


	void CleanCurPath()
	{
		cur_path_wp_index = 0;
	}
	/*
	void SetCurrentPath(LocalVector<NS_AI::WayPoint*> wp_list)
	{
		CleanCurPath();
		//way_point_list = wp_list;
	}*/

	WayPointComponent* GetCurPathWp()
	{
		if (cur_wp_path == nullptr || cur_wp_path->GetPath().size() == 0)
			return nullptr;
		return cur_wp_path->GetPath().at(path_indexes.at(cur_path_wp_index).first);
	}
	WayPointComponent* GetPrevPathWp()
	{
		if (cur_wp_path == nullptr || cur_wp_path->GetPath().size() == 0)
			return nullptr;
		return cur_wp_path->GetPath().at(path_indexes.at(prev_path_wp_index).first);
	}

	WayPointComponent* GetCurWalkingWp()
	{
		if (cur_wp_path == nullptr || cur_wp_path->GetPath().size() == 0)
			return nullptr;
		return cur_wp_path->GetPath().at(path_indexes.at(cur_route_wp_index).first);
	}
	WayPointComponent* GetPrevWalkingWp()
	{
		if (cur_wp_path == nullptr || cur_wp_path->GetPath().size() == 0)
			return nullptr;
		return cur_wp_path->GetPath().at(path_indexes.at(prev_route_wp_index).first);
	}

	float DistFromCurPathWP();
	float DistFromPrevPathWP();

	//LocalVector<int>
	bool MoreThenOneWPActive()
	{
		int act_wp_amt = 0;
		for (auto path : path_indexes)
		{
			if (path.second) //Check if path is active
			{
				++act_wp_amt;
				if (act_wp_amt > 1)
					return true;
			}
		}
		return false;
	}

	
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
			if (itr->name == "circuling_rad")
			{
				//size_in_rad = itr->value.GetFloat();
				circuling_rad = itr->value.GetFloat();
			}
			if (itr->name == "nav_state")
			{
				//size_in_rad = itr->value.GetFloat();
				nav_state = (NAV_STATE)itr->value.GetInt();
			}
			if (itr->name == "wp_creation_type")
			{
				wp_creation_type = (WP_PATH_CREATION_TYPE)itr->value.GetInt();
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
		NS_SERIALISER::ChangeData(&val, "circuling_rad", circuling_rad);
		NS_SERIALISER::ChangeData(&val, "nav_state", (int)nav_state);
		NS_SERIALISER::ChangeData(&val, "wp_creation_type", (int)wp_creation_type);
		return val;
	};
	virtual Value& Write(Value& val) { return val; };	
	
	virtual ComponentNavigator* Clone() {
		ComponentNavigator* newcomp = new ComponentNavigator();
		*newcomp = *this;
		return newcomp;
	}

	void InitPath()
	{
		Entity ent = G_ECMANAGER->getEntity(this);
		trans = ent.getComponent<TransformComponent>();
		rb = ent.getComponent<RigidBody>();

		if (cur_wp_path == nullptr)
		{
			if (!wp_path_ent_name.empty())
			{
				Entity path_ent = G_ECMANAGER->getEntityUsingEntName((std::string)wp_path_ent_name);
				if (path_ent.getId() != -1)
					cur_wp_path = path_ent.getComponent<WayPointMapComponent>();
				else
				{
					TracyMessageL("ComponentNavigator::InitPath : No entity have been found");
					SPEEDLOG("ComponentNavigator::InitPath : No entity have been found");
					return;
				}
			}
			else
			{
				TracyMessageL("ComponentNavigator::InitPath : No entity name have been found");
				SPEEDLOG("ComponentNavigator::InitPath : No entity name have been found");
				return;
			}
			//if way point path entity does not exist
			if (cur_wp_path == nullptr)
				return;
		}

		cur_route_wp_index = 0;
		prev_route_wp_index = 0;
		isPaused = false;

		switch (wp_creation_type) 
		{
		case WPP_STANDARD:
		{
			path_indexes.clear();
			size_t wp_size = cur_wp_path->GetPath().size();
			for (size_t i = 0; i < wp_size; ++i)
				path_indexes.push_back(std::make_pair((int)i, true));
			break;
		}
		case WPP_REVERSE:
		{
			path_indexes.clear();
			size_t wp_size = cur_wp_path->GetPath().size() - 1;
			for (int i = static_cast<int>(wp_size); i >= 0; --i)
				path_indexes.push_back(std::make_pair(i, true));
			break;
		}
		case WPP_CUSTOM:	//Inserted beforehand
		{
			break;
		}
		}
		//SetNextWp(this);
	}
	
	void StopAtEachWPCheck()
	{
		if (stopAtEachWayPoint)
		{
			isPaused = true;
			curTime = 0.0f;
		}
	}

	void ToGoToPrevWP()
	{
		if (!path_indexes.at(cur_route_wp_index).second && cur_route_wp_index!= 0)
			--cur_route_wp_index;
	}

	void DecideOnNextWp()
	{
		if (!HaveWayPoint()) //Dont need to move if one or less way point
			return;

		//if(wp_to_reach_end.size() == 0 || cur_route_wp_index == wp_to_reach_end.size())
			//Astar to find the route
			//return;


		prev_route_wp_index = cur_route_wp_index;
		if (wp_nav_type == WN_RANDOM)
		{
			srand((unsigned int)time(NULL));
			cur_route_wp_index = rand() % cur_wp_path->GetPath().size();
		}
		else
			if (traverseFront)
				if (cur_route_wp_index < path_indexes.size() - 1)
					++cur_route_wp_index;
				else {
					if (wp_nav_type == WN_TOANDFRO)
					{
						traverseFront = false;
						--cur_route_wp_index;
					}
					else if (wp_nav_type == WN_CIRCULAR)
					{
						cur_route_wp_index = 0;
					}
				}
			else
				if (cur_route_wp_index > 0)
					--cur_route_wp_index;
				else
				{
					traverseFront = true;
					++cur_route_wp_index;
				}

		if (!path_indexes.at(cur_route_wp_index).second)
			DecideOnNextWp();

		cur_path_wp_index = cur_route_wp_index;			//Temp ==
		prev_path_wp_index = prev_route_wp_index;
		//cur_path_wp_index = 0;.
		//prev_path_wp_index = 0;
	}

	//Function to set next way point to go to
	void SetNextWp(ComponentNavigator* nav)
	{
		nav;
		StopAtEachWPCheck();			//Check if stopping around a way point 
		DecideOnNextWp();				//Decide on which way point to go to next
		//2. Check for direct route

		//3. Find route to way point if being blocked
	}

	void GoToClosestWP();

	bool IsAroundWP(float proximity = 0);
	//================ Getter / Setter ========================//
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

	void TurnOffCurWayPoint()
	{
		path_indexes.at(cur_path_wp_index).second = false;
	}

	void ToggleWayPointActive(int index, bool act)
	{
		path_indexes.at(index).second = act;
	}



} NavigatorComponent, NavComponent;