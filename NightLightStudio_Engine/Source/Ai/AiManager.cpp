#include "AiManager.h"
#include "../Physics/SystemPhysics.h"
#include "../IO/Json/Config.h"
#include "../Core/DeltaTime.h" 

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
}

inline void NS_AI::AiManager::GameInit()
{
	auto wpm_itr = G_ECMANAGER->begin<WayPointMapComponent>();
	auto wpm_itrEnd = G_ECMANAGER->end<WayPointMapComponent>();

	while (wpm_itr != wpm_itrEnd)
	{
		WayPointMapComponent* wpmComp = reinterpret_cast<WayPointMapComponent*>(*wpm_itr);
		wpmComp->InitPath();
		++wpm_itr;
	}

	auto nc_itr = G_ECMANAGER->begin<NavigatorComponent>();
	auto nc_itrEnd = G_ECMANAGER->end<NavigatorComponent>();
	
	while (nc_itr != nc_itrEnd)
	{
		NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*nc_itr);
		navComp->InitPath();
		++nc_itr;
	}

	Obstacle_list = G_ECMANAGER->getEntityList("Obstacle");

}

inline void NS_AI::AiManager::Update()
{
	if (!isActive)
		return;
	//currentNumberOfSteps = 0;
	
	//Make shift dt
	std::swap(timeLastRound, timeThisRound);
	stepTime = timeThisRound = std::chrono::system_clock::now();
	float dt = (float)(timeThisRound - timeLastRound).count() / 10000000.0f;

	auto itr = G_ECMANAGER->begin<NavigatorComponent>();
	auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

	while (itr != itrEnd)
	{
		NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*itr);
		//AABBCollider* nav_aabb = &G_ECMANAGER->getComponent<ColliderComponent>(itr)->collider.aabb;
		
		if ( !navComp->HaveWayPoint() || !navComp->isFollowing)
		{
			++itr;
			continue;
		}

		TransformComponent* navTrans = G_ECMANAGER->getComponent<TransformComponent>(itr);
		RigidBody* rb = G_ECMANAGER->getComponent<RigidBody>(itr);

		if (navComp->isPaused) //&& navComp->curTime >= 0.0f)
		{
			rb->velocity = 0.0f;
			++itr;
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
			continue;
		}

		NlMath::Vector3D mag_dir = navComp->GetCurWp()->GetPos() - navTrans->_position ;	//Dir to the next way point
		mag_dir.y = 0.0f;	//Ignore y axis
		if (mag_dir.length() < navComp->size_in_rad)	//Check if Ai reached the way point
		{
			navComp->SetNextWp();		//Set next way point to be the target for navigation

			/*
			for (Entity& ent : Obstacle_list)
			{
				AABBCollider* obs_aabb = &ent.getComponent<ColliderComponent>()->collider.aabb;
				TransformComponent* trans_aabb = ent.getComponent<TransformComponent>();
				/*
				//If obstacle in the way between way point
				if (NlMath::RayToAABB(*obs_aabb, navTrans->_position, trans_aabb->_position))
				{
					NlMath::Vec3 points[] = {
						trans_aabb->_position +


					}
				}
			}*/
			std::cout << "Going to next wp" << std::endl;
			rb->velocity = 0.0f;
		}
		else
		{
			NlMath::Vector3DNormalize(mag_dir);
			//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(G_ECMANAGER->getEntity(itr),mag_dir, navComp->speed);	//Move to way point
			rb->velocity = mag_dir * navComp->speed;
		}
		++itr;
	}
}

inline void NS_AI::AiManager::WalkTowards(NavigatorComponent* nav_comp, NlMath::Vec3 my_pos, NlMath::Vec3 target_position)
{
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
