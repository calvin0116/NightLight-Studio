#pragma once
#include "../Core/MySystem.h"
//#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "../Math/Vector.h"
#include "../Core/DeltaTime.h"
#include "ForceManager.h"

namespace NS_PHYSICS
{
	constexpr float MAX_SPEED = 10000.0f;

	class PhysicsSystem : public MySystem, public Singleton<PhysicsSystem>
	{
		ForceManager _forceManager;
		NlMath::Vector3D _maxspeed;
		
		friend Singleton<PhysicsSystem>;
		
	public:
		
		ForceManager* getForceManager()
		{
			return &_forceManager;
		}
		
		PhysicsSystem();

		void OnFirstStart();

		void Load();

		void Init();

		void FixedUpdate() override;

		void Exit() override;

		void Free();

		void Unload();


	};

	static PhysicsSystem* SYS_PHYSICS = PhysicsSystem::GetInstance();
	static ForceManager& USE_THE_FORCE = *(SYS_PHYSICS->getForceManager());
}