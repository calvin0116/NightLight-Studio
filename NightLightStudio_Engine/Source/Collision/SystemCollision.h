#pragma once
#include "../Core/MySystem.h"
//#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "../Math/Vector.h"
#include "../Component/ComponentCollider.h"
#include "CollisionResolver.h"

namespace NS_COLLISION
{
	class CollisionSystem : public MySystem, public Singleton<CollisionSystem>
	{
		CollsionResolver colResolver;

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
		
		bool CheckCollision(ComponentCollider* Collider1, ComponentCollider* Collider2, ComponentRigidBody* Rigid1, ComponentRigidBody* Rigid2);

	private:

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// Line Mesh

		int MeshLod;
		bool doDrawLineMesh;

		void DrawLineMesh(ComponentTransform* comTrans, ComponentCollider* comCol, int lod = 0, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

		//// Line Mesh END
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	};

	static CollisionSystem* SYS_COLLISION = CollisionSystem::GetInstance();
}