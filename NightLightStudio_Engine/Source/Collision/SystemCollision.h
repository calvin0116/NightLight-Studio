#pragma once
#include "../Core/MySystem.h"
//#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "../Math/Vector.h"
#include "../Component/ComponentCollider.h"

namespace NS_COLLISION
{
	class ENGINE_API CollisionSystem : public MySystem, public Singleton<CollisionSystem>
	{

		friend Singleton<CollisionSystem>;

	public:


		CollisionSystem();

		void Load() override;

		void Init() override;

		void GameLoad() override;

		void GameInit() override;

		void Update() override;

		void GameExit() override;

		void Exit() override;

		void Free() override;

		ICollider* GetCollider(ComponentCollider* collider);

		void UpdateCollisionBoxPos(ComponentCollider * comCol, ComponentTransform * comTrans);
		
		bool CheckCollision(ComponentCollider* Collider1 , ComponentCollider* Collider2);


	};

	static CollisionSystem* SYS_COLLISION = CollisionSystem::GetInstance();
}