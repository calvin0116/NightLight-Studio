#include "AiManager.h"
#include "../Physics/SystemPhysics.h"
#include "../IO/Json/Config.h"
#include "../Core/DeltaTime.h" 

// Tracy
#include "../tracy-master/Tracy.hpp"

inline void NS_AI::AiManager::HandleMsg(MessageTogglePlay& mst)
{
	if (mst.GetID() == "BeforePlay")
	{
		// Game started playing
		//TempSave();
		GameInit();
		isActive = true;
	}
	else
	{
		isActive = false;
	}
}

inline void NS_AI::AiManager::Init()
{
	r.AttachHandler("BeforePlay", &AiManager::HandleMsg, this);
	r.AttachHandler("AfterPlay", &AiManager::HandleMsg, this);

	if (CONFIG_DATA->GetConfigData().levelEditorMode)
		isActive = false;

	timeLastRound = timeThisRound = std::chrono::system_clock::now();




	//wp_man.Init();
}

inline void NS_AI::AiManager::GameInit()
{
	auto trans_itr = G_ECMANAGER->begin<TransformComponent>();
	auto trans_itrEnd = G_ECMANAGER->end<TransformComponent>();
	while (trans_itr != trans_itrEnd)
	{
		TransformComponent* trans_Comp = reinterpret_cast<TransformComponent*>(*trans_itr);
		for (int& tagID : trans_Comp->_tagNames)
			if (TAGNAMES[tagID] == S_OBS)
				InsertObstacle(G_ECMANAGER->getEntity(trans_itr).getComponent<ColliderComponent>());

		++trans_itr;
	}

	auto wpm_itr = G_ECMANAGER->begin<WayPointMapComponent>();
	auto wpm_itrEnd = G_ECMANAGER->end<WayPointMapComponent>();

	while (wpm_itr != wpm_itrEnd)
	{
		WayPointMapComponent* wpmComp = reinterpret_cast<WayPointMapComponent*>(*wpm_itr);
		wpmComp->InitPath();
		++wpm_itr;
	}

	wp_man.GameInit();

	auto nc_itr = G_ECMANAGER->begin<NavigatorComponent>();
	auto nc_itrEnd = G_ECMANAGER->end<NavigatorComponent>();
	
	while (nc_itr != nc_itrEnd)
	{
		NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*nc_itr);
		
		navComp->InitPath();
		++nc_itr;
	}

	//Obstacle_list = G_ECMANAGER->getEntityList("Obstacle");

}

inline void NS_AI::AiManager::Update()
{
	wp_man.Update();

	if (!isActive)
		return;
	//currentNumberOfSteps = 0;
	//std::chrono::duration_cast<std::chrono::microseconds>();
	auto itr = G_ECMANAGER->begin<NavigatorComponent>();
	auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

	while (itr != itrEnd)
	{
		NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*itr);
		NavBehaviour(navComp);
		++itr;
	}

	// Tracy
	// Zone color: Orange
	ZoneScopedNC("AI", 0xff8f26);
}

inline void NS_AI::AiManager::WalkTowards(NavigatorComponent* nav_comp, NlMath::Vec3 my_pos, NlMath::Vec3 target_position)
{
	target_position;
	nav_comp;
	my_pos;
	if (wp_man.GetWayPointNumber())	//If there is way point
	{
		//nav_comp->SetCurPathIndex(wp_man.AstarPathFinding(my_pos, target_position,nav_comp->GetCurPath()));
	}
	else
	{
		//LocalVector<WayPoint*> temp_wp_list;
		//wp_man.InsertWayPoint(my_pos, (my_pos - target_position).length());
		//nav_comp->SetCurrentPath
	}
}

inline void NS_AI::AiManager::WalkTowards(int ent_id, NlMath::Vec3 target_position)
{
	Entity ent = G_ECMANAGER->getEntity(ent_id);
	NavigatorComponent* nav_comp = ent.getComponent<NavigatorComponent>();
	TransformComponent* trans_comp = ent.getComponent<TransformComponent>();

	WalkTowards(nav_comp, trans_comp->_position, target_position);
}

void NS_AI::AiManager::NavBehaviour(NavigatorComponent* navComp)
{
	//Make shift dt
	std::swap(timeLastRound, timeThisRound);
	stepTime = timeThisRound = std::chrono::system_clock::now();
	float dt = (float)(timeThisRound - timeLastRound).count() / 10000000.0f;

	//Check if there is way point
	if (!navComp->isFollowing || navComp->GetCurWalkingWp() == nullptr)
	{
		return;
	}

	TransformComponent* navTrans = navComp->trans;
	RigidBody* rb = navComp->rb;

	//Paused
	if (navComp->isPaused) //&& navComp->curTime >= 0.0f)
	{
		rb->velocity = 0.0f;
		if (navComp->endTime <= 0.0f)  //Check if infinitely or with timing given
			return;

		//++itr;
		if (navComp->curTime >= 0.0f)
			navComp->curTime += dt;//(DELTA_T->real_dt)*0.001;
		//else
			//std::cout << "isPaused" << std::endl;
		//std::cout << navComp->curTime  << std::endl;
		if (navComp->curTime > navComp->endTime)
		{
			navComp->isPaused = false;
			//navComp->curTime = -1.f;
			navComp->curTime = -1.0f;
		}
		return;
	}
	
	if (navTrans == nullptr)
		return;
	//TracyMessageL("AiManager::NavBehavior: Why are you here alr?");
	//Determine next way point and whether to move towards it out not
	glm::vec3 wp_pos = navComp->GetCurWalkingWp()->GetPos();
	//wp_pos.y = 0.0f;
	navComp->dir = wp_pos - navTrans->_position;	//Dir to the next way point
	//mag_dir.y = 0.0f;	//Ignore y axis

	switch (navComp->nav_state)
	{
		case NV_PATROL:
		{
			float len = glm::length(navComp->dir);
			if (len < navComp->size_in_rad)	//Check if Ai reached the way point
			{
				navComp->SetNextWp(nullptr);//Set next way point to be the target for navigation
				navComp->dir = glm::normalize(navComp->dir);
				//std::cout << "Going to next wp" << std::endl;
				TracyMessageL("AiManager::NavBehavior: Going to next wp");
				rb->velocity = 0.0f;
			}
			else
			{
				navComp->dir = glm::normalize(navComp->dir);
				//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(G_ECMANAGER->getEntity(itr),mag_dir, navComp->speed);	//Move to way point
				rb->velocity = navComp->dir * navComp->speed;
			}
			break;
		}
		case NV_CIRCLING:
		{
			float len = glm::length(navComp->dir);
			if ((len - navComp->circuling_rad) < FLT_EPSILON)	//Check if Ai reached the way point
			{
				//Circular motion
				glm::vec3 rev_dir = navTrans->_position - wp_pos;
				rev_dir.y = 0.0f;
				glm::quat Quaternion(glm::vec3(0.0f, 1.5708f, 0.0f));
				glm::mat4 Rotate = glm::mat4_cast(Quaternion);

				glm::vec3 dir = Rotate * glm::vec4{ rev_dir , 1.0f };
				dir = glm::normalize(dir);
				rb->velocity = dir * navComp->speed;
				navComp->dir = glm::normalize(dir);
			}

			else
			{
				navComp->dir = glm::normalize(navComp->dir);
				//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(G_ECMANAGER->getEntity(itr),mag_dir, navComp->speed);	//Move to way point
				rb->velocity = navComp->dir * navComp->speed;
			}
			break;
		}
	}
}

//Select closest way point to move to
/*
inline void NS_AI::AiManager::FindClosestWP(NavigatorComponent* nav_comp, TransformComponent* trans_comp)
{
	std::pair<int, float> closest_wp = std::make_pair<int, float>(-1, std::numeric_limits<float>::max());

	int i = 0;
	for (; i < nav_comp->GetCurPath().size(); ++i)
	{
		auto wp = nav_comp->GetCurPath().at(i);

		float dist = static_cast<float>((wp->GetPos() - trans_comp->_position).length());
		if (dist < closest_wp.second)
		{
			closest_wp.first = i;
			closest_wp.second = dist;
		}
	}

	nav_comp->cur_route_wp_index = i;	//wp index to go to will be the closest one
}*/
