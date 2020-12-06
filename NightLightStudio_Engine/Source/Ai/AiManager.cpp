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
	auto itr = G_ECMANAGER->begin<NavigatorComponent>();
	auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

	while (itr != itrEnd)
	{
		NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*itr);
		navComp->InitPath();
		++itr;
	}

	Obstacle_list = G_ECMANAGER->getEntityList("Obstacle");

}

inline void NS_AI::AiManager::Update()
{
	if (!isActive)
		return;
	//currentNumberOfSteps = 0;

	std::swap(timeLastRound, timeThisRound);
	stepTime = timeThisRound = std::chrono::system_clock::now();
	float dt = (float)(timeThisRound - timeLastRound).count() / 10000000.0f;

	auto itr = G_ECMANAGER->begin<NavigatorComponent>();
	auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

	while (itr != itrEnd)
	{
		NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*itr);
		//AABBCollider* nav_aabb = &G_ECMANAGER->getComponent<ColliderComponent>(itr)->collider.aabb;
		
		if (navComp->WPSize() == 0  || !navComp->isFollowing)
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

		NlMath::Vector3D mag_dir = navComp->GetCurWp()->_position - navTrans->_position ;	//Dir to the next way point
		mag_dir.y = 0.0f;	//Ignore y axis
		//5.0f = hardcoded radius to check
		//std::cout << mag_dir.length() << std::endl;
		if (mag_dir.length() < navComp->rad_for_detect)	//Check if Ai reached the way point
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
