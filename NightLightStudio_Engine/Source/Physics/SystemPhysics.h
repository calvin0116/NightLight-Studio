#pragma once
#include "../Core/MySystem.h"
//#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "../Math/Vector.h"
#include "../Core/DeltaTime.h"

namespace NS_PHYSICS
{
	class PhysicsSystem : public MySystem, public Singleton<PhysicsSystem>
	{
		NlMath::Vector3D _maxspeed;
		float gravity;
		friend Singleton<PhysicsSystem>;
		
	public:
		
		
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
}