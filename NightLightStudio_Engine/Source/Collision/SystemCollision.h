#pragma once
#include <vector>

#include "../Core/MySystem.h"
//#include "../IO/Json/Config.h"
#include "../../framework.h"
#include "../Math/Vector.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentRigidBody.h"
#include "CollisionResolver.h"
#include "../Component/ComponentManager.h"
#include "CSVReader.h"

//messaging for toggle play
#include "../Messaging/SystemReceiver.h"
#include "../Messaging/Messages/MessageScriptRequest.h"
#include "../Messaging/Messages/MessageApplicationExit.h"
#include "../Messaging/Messages/MessageTogglePlay.h"

namespace NS_COLLISION
{
	class CollisionSystem : public MySystem, public Singleton<CollisionSystem>
	{
		CollsionResolver colResolver;

		//for reading data in collider table
		std::vector<std::vector<std::string> > _colliderDataList;

		//toggeling playable
		bool _isPlaying;
		friend Singleton<CollisionSystem>;

		SystemMessaging::SystemReceiver receiver;
	public:


		CollisionSystem();

		void Load() override;

		void Init() override;

		void GameLoad() override;

		void GameInit() override;

		void FixedUpdate() override;

		void GameExit() override;

		void Exit() override;

		void Free() override;

		ICollider* GetCollider(ComponentCollider* collider);

		void UpdateCollisionBoxPos(ComponentCollider* comCol, ComponentTransform* comTrans);
		bool CollisionCuling(ComponentTransform* comTrans1, ComponentTransform* comTrans2);
		bool CheckTrigger(ComponentCollider* Collider1, ComponentCollider* Collider2);
		bool CheckCollision(
			ComponentCollider* Collider1, ComponentCollider* Collider2,
			ComponentRigidBody* Rigid1, ComponentRigidBody* Rigid2,
			ComponentTransform* Transform1, ComponentTransform* Transform2,
			Entity Entity1, Entity Entity2
		);
		void HandleTogglePlay(MessageTogglePlay& msg);

		////
		// ray thingy
		bool Check_RayCollider(ComponentCollider* collider, NlMath::Vec3 rayOrigin, NlMath::Vec3 rayEnd, NlMath::Vec3& intersect);

		bool Check_RayCollision(NlMath::Vec3 rayOrigin, NlMath::Vec3 rayEnd, NlMath::Vec3& intersect);

		void Test_Ray(NlMath::Vec3 rayOrigin, NlMath::Vec3 rayEnd);

		void Draw3DCross(NlMath::Vec3 point, float size, NlMath::Vec3 color);
		//
		////

	private:

		// test var
		ComponentTransform* compTtest0;

		ComponentRigidBody* compRtest0;

		Entity entityTest0;

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