#include "AiManager.h"
#include "../Physics/SystemPhysics.h"
#include "../IO/Json/Config.h"

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

}

inline void NS_AI::AiManager::Update()
{
	if (!isActive)
		return;

	auto itr = G_ECMANAGER->begin<NavigatorComponent>();
	auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

	while (itr != itrEnd)
	{
		NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*itr);
		TransformComponent* trans = G_ECMANAGER->getComponent<TransformComponent>(itr);
		RigidBody* rb = G_ECMANAGER->getComponent<RigidBody>(itr);
		
		if (navComp->WPSize() == 0 || navComp->isPaused || !navComp->isFollowing)
		{
			++itr;
			continue;
		}
		NlMath::Vector3D mag_dir = navComp->GetCurWp()->_position - trans->_position ;	//Dir to the next way point
		mag_dir.y = 0.0f;	//Ignore y axis
		//5.0f = hardcoded radius to check
		std::cout << mag_dir.length() << std::endl;
		if (mag_dir.length() < 25.0f)	//Check if Ai reached the way point
		{
			navComp->SetNextWp();		//Set next way point to be the target for navigation
			std::cout << "Going to next wp" << std::endl;
		}
		NlMath::Vector3DNormalize(mag_dir);
		//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(G_ECMANAGER->getEntity(itr),mag_dir, navComp->speed);	//Move to way point
		rb->velocity = mag_dir * navComp->speed;
		++itr;
	}
}