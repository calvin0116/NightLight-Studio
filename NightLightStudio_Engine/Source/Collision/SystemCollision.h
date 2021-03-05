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

		void FixedUpdate(float dt) override;

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
		bool Check_RayCollider(ComponentCollider* collider, NlMath::Vec3 rayOrigin, NlMath::Vec3 rayEnd, NlMath::Vec3& intersect, float& iSqLen);

		int Check_RayCollision(NlMath::Vec3 rayOrigin, NlMath::Vec3 rayEnd, NlMath::Vec3& intersect);

		int Check_RayCollision(NlMath::Vec3 rayOrigin, NlMath::Vec3 rayEnd, NlMath::Vec3& intersect, int k);

		void Test_Ray(NlMath::Vec3 rayOrigin, NlMath::Vec3 rayEnd);

		void Draw3DCross(NlMath::Vec3 point, float size, NlMath::Vec3 color);
		//
		////

		////
		// snap thingy
		NlMath::Vec3 currentSnapVec;

	private:
		const int DIMENSIONS = 3;
		enum class DIRECTION
		{
			X_DIRECTION = 0,
			Y_DIRECTION = 1,
			Z_DIRECTION = 2,
			YDOWN_DIRECTION = 3
		};
	public:

#define SNAP_X 1
#define SNAP_Y 2
#define SNAP_Z 4
#define SNAP_D 8

		bool Snap_AABB_AABB_Detect(Entity entity, float distance, int directions, int lod, bool isDrawDebug);

		bool Snap_AABB_AABB_Do(Entity entity, float distance, float gap, int directions, int lod, bool isDrawDebug);

	private:
		bool Snap_To_AABB(Entity entity, float distance, DIRECTION direction, NlMath::Vec3& snapVec, int lod, bool isDrawDebug);
		bool Snap_FourCast(
			NlMath::Vec3 vecA, NlMath::Vec3 vecB,
			NlMath::Vec3 vecC, NlMath::Vec3 vecD,
			float distance, NlMath::Vec3 castDir,
			NlMath::Vec3& snapVec,
			int lod, bool isDrawDebug);

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