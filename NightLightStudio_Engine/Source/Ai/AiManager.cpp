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

		//Paused
		if (navComp->isPaused) //&& navComp->curTime >= 0.0f)
		{
			rb->velocity = 0.0f;
			if (navComp->endTime <= 0.0f)  //Check if infinitely or with timing given
				continue;

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

		glm::vec3 wp_pos = navComp->GetCurWp()->GetPos();
		wp_pos.y = 0.0f;
		glm::vec3 mag_dir = wp_pos - navTrans->_position;	//Dir to the next way point
		mag_dir.y = 0.0f;	//Ignore y axis
		switch (navComp->nav_state)
		{
			case NV_PATROL:
			{
				float len = glm::length(mag_dir);
				if (len < navComp->size_in_rad)	//Check if Ai reached the way point
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
					mag_dir = glm::normalize(mag_dir);
					//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(G_ECMANAGER->getEntity(itr),mag_dir, navComp->speed);	//Move to way point
					rb->velocity = mag_dir * navComp->speed;
				}
				break;
			}
			case NV_CIRCLING:
			{
				float len = glm::length(mag_dir);
				if ((len-navComp->circuling_rad) < FLT_EPSILON)	//Check if Ai reached the way point
				{
					//Circular motion
					//Caculate the angle from the way point
					glm::vec3 up_vec = { 1.0f, 0.f, 0.f };
					glm::vec3 rev_dir = navTrans->_position - wp_pos;
					float ab_dot = glm::dot(rev_dir, up_vec);
					float ab_mag = up_vec.length() * rev_dir.length();
					 
					float f = fabsf(ab_dot) / ab_mag;
					float angle = glm::acos(glm::radians(f));
						
					//Create the matrix
					glm::mat4 BackToCenter = glm::identity<glm::mat4>();
					BackToCenter = glm::translate(BackToCenter, glm::vec3(-wp_pos.x, 0.0f, -wp_pos.z));

					glm::quat Quaternion(glm::radians(glm::vec3(0.0f, angle +  0.1f, 0.0f)));
					glm::mat4 Rotate = BackToCenter * glm::mat4_cast(Quaternion);

					glm::mat4 TranslateBack = glm::translate(Rotate, wp_pos);

					glm::vec3 new_pos = TranslateBack * glm::vec4{ navTrans->_position , 1.0f};
					//auto rotatedVec = ;

					rb->velocity = (new_pos - navTrans->_position);
				}
				else
				{
					mag_dir = glm::normalize(mag_dir);
					//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(G_ECMANAGER->getEntity(itr),mag_dir, navComp->speed);	//Move to way point
					rb->velocity = mag_dir * navComp->speed;
				}
				break;
			}
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
