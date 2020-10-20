#include "SystemCollision.h"
#include "../Core/DeltaTime.h"
#include "../Graphics/GraphicsSystem.h"//for testing, to be removed
#include "../Component/Components.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentRigidBody.h"
#include "../Input/SystemInput.h"

namespace NS_COLLISION
{
	CollisionSystem::CollisionSystem()
	{
	}
	void CollisionSystem::Load()
	{
	}
	void CollisionSystem::Init()
	{
		colResolver.clear();


		//test code, to be removed
		//////////////////////////////////////////////////////////////////////////////////////
		//test creation
		Entity cube1Test = G_ECMANAGER->BuildEntity();
		ComponentTransform Transform1;
		ComponentRigidBody Rigid1;
		

		Rigid1.isStatic = false;
		//Transform1._rotation.x = 45;
		//Transform1._rotation.y = 45;
		//Transform1._rotation.z = 45;

		cube1Test.AttachComponent<ComponentTransform>(Transform1);
		cube1Test.AttachComponent<ComponentRigidBody>(Rigid1);
		


		NS_GRAPHICS::SYS_GRAPHICS->CreateCube(cube1Test, glm::vec3(0.5f, 0.5f, 1.f));

		Entity cube2Test = G_ECMANAGER->BuildEntity();
		ComponentTransform Transform2;
		ComponentRigidBody Rigid2;
		

		Transform2._position = glm::vec3(2.5f, 2.5f, 1.f);
		//Transform2._rotation.x = 45;
		//Transform2._rotation.y = 45;
		//Transform1._rotation.z = 45;


		cube2Test.AttachComponent<ComponentTransform>(Transform2);
		cube2Test.AttachComponent<ComponentRigidBody>(Rigid2);
		
		
		
		NS_GRAPHICS::SYS_GRAPHICS->CreateCube(cube2Test, glm::vec3(1.0f, 0.0f, 1.f));

		ComponentCollider AABB1(COLLIDERS::AABB);
		ComponentCollider AABB2(COLLIDERS::AABB);
		cube1Test.AttachComponent<ComponentCollider>(AABB1);
		cube2Test.AttachComponent<ComponentCollider>(AABB2);

		//ComponentCollider OBB1(COLLIDERS::OBB);
		//ComponentCollider OBB2(COLLIDERS::OBB);
		//cube1Test.AttachComponent<ComponentCollider>(OBB1);
		//cube2Test.AttachComponent<ComponentCollider>(OBB2);


		
		//////////////////////////////////////////////////////////////////////////////////////
	}

	void CollisionSystem::GameLoad()
	{
	}

	void CollisionSystem::GameInit()
	{
		auto itr = G_ECMANAGER->begin<ComponentCollider>();
		auto itrEnd = G_ECMANAGER->end<ComponentCollider>();
		for (; itr != itrEnd; ++itr)
		{
			ComponentCollider* comCol = G_ECMANAGER->getComponent<ComponentCollider>(itr);
			ComponentTransform* comTrans = G_ECMANAGER->getComponent<ComponentTransform>(itr);

			UpdateCollisionBoxPos(comCol, comTrans);
		}
	}

	void CollisionSystem::Update()
	{
		auto itr1 = G_ECMANAGER->begin<ComponentCollider>();
		auto itrEnd1 = G_ECMANAGER->end<ComponentCollider>();

		auto itr2 = G_ECMANAGER->begin<ComponentCollider>();
		auto itrEnd2 = itrEnd1;

		for (; itr1 != itrEnd1; ++itr1)
		{
			for (itr2 = itr1 ; itr2 != itrEnd2; ++itr2)
			{
				if (itr1 == itr2)
				{
					continue;
				}
				//potential issue if both component belong to the same entity, need a detection here for same entity id
				Entity Ent1 = G_ECMANAGER->getEntity(itr1);
				Entity Ent2 = G_ECMANAGER->getEntity(itr2);
				if (Ent1.getId() == Ent2.getId())
				{
					continue;
				}
				
				//Get colliders
				ComponentCollider* comCol1 = G_ECMANAGER->getComponent<ComponentCollider>(itr1);
				ComponentCollider* comCol2 = G_ECMANAGER->getComponent<ComponentCollider>(itr2);

				//Get Transforms
				ComponentTransform* comTrans1 = G_ECMANAGER->getComponent<ComponentTransform>(itr1);

				//Get rigidBody for Collision Resolution
				ComponentRigidBody* comRigid1 = G_ECMANAGER->getComponent<ComponentRigidBody>(itr1);
				ComponentRigidBody* comRigid2 = G_ECMANAGER->getComponent<ComponentRigidBody>(itr2);

				UpdateCollisionBoxPos(comCol1, comTrans1);

				
				//check for collision, also create collision event in CheckCollision if there is collision
				if (CheckCollision(comCol1, comCol2, comRigid1, comRigid2))
				{
					// store collision event
					NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent1, glm::vec3(1.0f, 0.0f, 1.f));
					NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent2, glm::vec3(0.8f, 0.0f, 1.f));
				}
				else
				{
					NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent1, glm::vec3(0.5f, 0.5f, 1.f));
					NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent2, glm::vec3(1.0f, 0.0f, 1.f));
				}

				//test code, to be romoved
				/////////////////////////////////////////////////////////////////////////////////////////
				//comTrans1->_rotation.x += 1;
				//comTrans1->_rotation.y += 1;
				//comTrans1->_rotation.z += 1;
				/////////////////////////////////////////////////////////////////////////////////////////
			}
		}

		//resolve collision here
		colResolver.resolveCollision();
	}
	void CollisionSystem::GameExit()
	{
	}
	void CollisionSystem::Exit()
	{
		DestroyInstance();
	}
	void CollisionSystem::Free()
	{
	}

	ICollider* CollisionSystem::GetCollider(ComponentCollider* collider)
	{
		switch (collider->colliderType)
		{
		case COLLIDERS::AABB:
			return &(collider->collider.aabb);

		default:
			break;
		}
		return nullptr;
	}

	void CollisionSystem::UpdateCollisionBoxPos(ComponentCollider* comCol, ComponentTransform* comTrans)
	{
		//update collision box position

		switch (comCol->colliderType)
		{
		case COLLIDERS::PLANE:
		{
			PlaneCollider* tmp = &(comCol->collider.plane);
			tmp->init(comTrans);
			break;
		}
		case COLLIDERS::SPHERE:
		{
			SphereCollider* tmp = &(comCol->collider.sphere);
			tmp->init(comTrans);
			break;
		}
		case COLLIDERS::AABB:
		{
			AABBCollider* tmp = &(comCol->collider.aabb);
			tmp->init(comTrans);
			break;
		}
		case COLLIDERS::OBB:
		{
			OBBCollider* tmp = &(comCol->collider.obb);
			tmp->init(comTrans);
			break;
		}
		case COLLIDERS::CAPSULE:
		{
			CapsuleCollider* tmp = &(comCol->collider.capsule);
			tmp->init(comTrans);
			break;
		}
		default:
			throw;
			break;
		}

	}

	bool CollisionSystem::CheckCollision(
		ComponentCollider* Collider1, ComponentCollider* Collider2, 
		ComponentRigidBody* Rigid1, ComponentRigidBody* Rigid2)
	{
		if (Collider1->colliderType == COLLIDERS::AABB)
		{
			if (Collider2->colliderType == COLLIDERS::AABB)
			{
				AABBCollider * a = &(Collider1->collider.aabb);
				AABBCollider * b = &(Collider2->collider.aabb);

				CollisionEvent newEvent;
				newEvent.colidingSide = NlMath::AABB_AABBCollision(*a, *b);
				if (newEvent.colidingSide == SIDES::NO_COLLISION)
				{
					return false;
				}
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;
				colResolver.addCollisionEvent(newEvent);
				return true;
				
			}
			//if (Collider2->colliderType == COLLIDERS::CAPSULE)
			//{
			//	AABBCollider* a = reinterpret_cast<AABBCollider*>(Collider1);
			//	CapsuleCollider* b = reinterpret_cast<CapsuleCollider*>(Collider2);
			//	//return NlMath::AABBtoAABB(*a, *b);
			//}
			//if (Collider2->colliderType == COLLIDERS::OBB)
			//{
			//	AABBCollider* a = reinterpret_cast<AABBCollider*>(Collider1);
			//	OBBCollider* b = reinterpret_cast<OBBCollider*>(Collider2);
			//	//return NlMath::AABBtoAABB(*a, *b);
			//}
			//if (Collider2->colliderType == COLLIDERS::PLANE)
			//{
			//	AABBCollider* a = reinterpret_cast<AABBCollider*>(Collider1);
			//	PlaneCollider* b = reinterpret_cast<PlaneCollider*>(Collider2);
			//	//return NlMath::AABBtoAABB(*a, *b);
			//}
			//if (Collider2->colliderType == COLLIDERS::SPHERE)
			//{
			//	AABBCollider* a = reinterpret_cast<AABBCollider*>(Collider1);
			//	SphereCollider* b = reinterpret_cast<SphereCollider*>(Collider2);
			//	//return NlMath::AABBtoAABB(*a, *b);
			//}

		}
		if (Collider1->colliderType == COLLIDERS::OBB)
		{
			if (Collider2->colliderType == COLLIDERS::OBB)
			{
				OBBCollider* a = &(Collider1->collider.obb);
				OBBCollider* b = &(Collider2->collider.obb);

				return NlMath::OBBToOBB(*a, *b);

			}
		}
		return false;
	}
}