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
		//Transform1._rotation.y = 90;
		//Transform1._rotation.z = 45;	 

		cube1Test.AttachComponent<ComponentTransform>(Transform1);
		cube1Test.AttachComponent<ComponentRigidBody>(Rigid1);
		


		NS_GRAPHICS::SYS_GRAPHICS->CreateCube(cube1Test, glm::vec3(0.5f, 0.5f, 1.f));

		Entity cube2Test = G_ECMANAGER->BuildEntity();
		ComponentTransform Transform2;
		ComponentRigidBody Rigid2;
		

		Transform2._position = glm::vec3(2.5f, 0.0f, 0.f);
		//Transform2._rotation.x = 45;
		//Transform2._rotation.y = 45;
		//Transform2._rotation.z = 45;
		//Transform2._scale.x = 2;


		cube2Test.AttachComponent<ComponentTransform>(Transform2);
		cube2Test.AttachComponent<ComponentRigidBody>(Rigid2);
		
		
		
		NS_GRAPHICS::SYS_GRAPHICS->CreateCube(cube2Test, glm::vec3(1.0f, 0.0f, 1.f));

		ComponentCollider AABB1(COLLIDERS::AABB);
		//ComponentCollider AABB2(COLLIDERS::AABB);

		//ComponentCollider AABB1(COLLIDERS::OBB);
		//ComponentCollider AABB2(COLLIDERS::OBB);

		//ComponentCollider AABB1(COLLIDERS::SPHERE);
		ComponentCollider AABB2(COLLIDERS::SPHERE);

		//ComponentCollider AABB1(COLLIDERS::CAPSULE);
		//ComponentCollider AABB2(COLLIDERS::CAPSULE);

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
				///////////////////////////////////////////////////////////////////////////////////
				//// AABB to AABB
				AABBCollider * a = &(Collider1->collider.aabb);
				AABBCollider * b = &(Collider2->collider.aabb);

				CollisionEvent newEvent;
				newEvent.colidingSide = NlMath::AABB_AABBCollision(*a, *b);
				if (newEvent.colidingSide == SIDES::NO_COLLISION)
				{
					return false; // no collision return false
				}
				// yes collision add to collision event and return true
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;
				newEvent.collisionType = COLRESTYPE::AABB_AABB;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// AABB to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::SPHERE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// AABB to SPHERE
				AABBCollider* a = &Collider1->collider.aabb;
				SphereCollider* b = &Collider2->collider.sphere;

				CollisionEvent newEvent;
				newEvent.colidingSide = NlMath::AABB_SphereCollision(*a, *b, newEvent.collisionNormal);
				if (newEvent.colidingSide == SIDES::NO_COLLISION)
				{
					return false; // no collision return false
				}
				// yes collision add to collision event and return true
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;
				newEvent.collisionType = COLRESTYPE::AABB_SPHERE;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// AABB to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::CAPSULE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// AABB to CAPSULE
				AABBCollider* a = &Collider1->collider.aabb;
				CapsuleCollider* b = &Collider2->collider.capsule;

				CollisionEvent newEvent;
				newEvent.colidingSide = NlMath::AABBToCapsule(*a, *b, newEvent.collisionNormal);
				if (newEvent.colidingSide == SIDES::NO_COLLISION)
				{
					return false; // no collision return false
				}
				// yes collision add to collision event and return true
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;
				newEvent.collisionType = COLRESTYPE::AABB_CAPSULE;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// AABB to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::OBB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// AABB to OBB

				//AABBCollider* a = reinterpret_cast<AABBCollider*>(Collider1);
				//OBBCollider* b = reinterpret_cast<OBBCollider*>(Collider2);
				//return NlMath::AABBtoAABB(*a, *b);

				//// AABB to OBB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			//if (Collider2->colliderType == COLLIDERS::PLANE)
			//{
			//	AABBCollider* a = reinterpret_cast<AABBCollider*>(Collider1);
			//	PlaneCollider* b = reinterpret_cast<PlaneCollider*>(Collider2);
			//	//return NlMath::AABBtoAABB(*a, *b);
			//}
		}
		if (Collider1->colliderType == COLLIDERS::SPHERE)
		{

			if (Collider2->colliderType == COLLIDERS::AABB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// SPHERE to AABB
				SphereCollider* a = &Collider1->collider.sphere;
				AABBCollider* b = &Collider2->collider.aabb;

				CollisionEvent newEvent;
				newEvent.colidingSide = NlMath::AABB_SphereCollision(*b, *a, newEvent.collisionNormal);
				if (newEvent.colidingSide == SIDES::NO_COLLISION)
				{
					return false; // no collision return false
				}
				// yes collision add to collision event and return true
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;
				newEvent.collisionType = COLRESTYPE::SPHERE_AABB;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// SPHERE to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::SPHERE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// SPHERE to SPHERE
				SphereCollider* a = &Collider1->collider.sphere;
				SphereCollider* b = &Collider2->collider.sphere;

				NlMath::Vector3D distance = NlMath::Sphere_SphereCollision(*a, *b);

				if (distance.x == 0.0f && distance.y == 0.0f && distance.z == 0.0f)
				{
					return false; // no collision return false
				}

				// yes collision add to collision event and return true

				CollisionEvent newEvent;

				// get nroamel
				newEvent.collisionNormal = -distance;

				// add rigid body
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;

				// set type
				newEvent.collisionType = COLRESTYPE::SPHERE_SPHERE;

				// add event
				colResolver.addCollisionEvent(newEvent);

				// return true
				return true;
				//// SPHERE to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::CAPSULE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// SPHERE to CAPSULE
				SphereCollider* a = &Collider1->collider.sphere;
				CapsuleCollider* b = &Collider2->collider.capsule;

				// TODO !!! SPHERE to CAPSULE

				//// SPHERE to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::OBB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// SPHERE to OBB

				//// SPHERE to OBB END
				///////////////////////////////////////////////////////////////////////////////////
			}
		}
		if (Collider1->colliderType == COLLIDERS::CAPSULE)
		{

			if (Collider2->colliderType == COLLIDERS::AABB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// CAPSULE to AABB
				CapsuleCollider* a = &Collider1->collider.capsule;
				AABBCollider* b = &Collider2->collider.aabb;

				CollisionEvent newEvent;
				newEvent.colidingSide = NlMath::AABBToCapsule(*b, *a, newEvent.collisionNormal);
				if (newEvent.colidingSide == SIDES::NO_COLLISION)
				{
					return false; // no collision return false
				}
				// yes collision add to collision event and return true
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;
				newEvent.collisionType = COLRESTYPE::CAPSULE_AABB;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// CAPSULE to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::SPHERE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// CAPSULE to SPHERE
				CapsuleCollider* a = &Collider1->collider.capsule;
				SphereCollider* b = &Collider2->collider.sphere;

				// TODO !!! SPHERE to CAPSULE

				//// CAPSULE to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::CAPSULE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// CAPSULE to CAPSULE
				CapsuleCollider* a = &Collider1->collider.capsule;
				CapsuleCollider* b = &Collider2->collider.capsule;

				CollisionEvent newEvent;
				bool check = NlMath::CapsuleToCapsule(*a, *b, newEvent.collisionNormal);
				if (check == false)
				{
					return false;// no collision return false
				}

				// yes collision add to collision event and return true

				// im not sure if the nroamel is correct !!!

				// add rigid body
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;

				// set type
				newEvent.collisionType = COLRESTYPE::CAPSULE_CAPSULE;

				// add event
				colResolver.addCollisionEvent(newEvent);

				// return true
				return true;
				//// CAPSULE to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::OBB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// CAPSULE to OBB

				//// CAPSULE to OBB END
				///////////////////////////////////////////////////////////////////////////////////
			}
		}
		if (Collider1->colliderType == COLLIDERS::OBB)
		{
			if (Collider2->colliderType == COLLIDERS::OBB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to OBB

				OBBCollider* a = &(Collider1->collider.obb);
				OBBCollider* b = &(Collider2->collider.obb);

				NlMath::Vector3D normal;
				bool check = NlMath::OBBToOBB(*a, *b, normal);

				// debug lines?
				NlMath::Vector3D start = a->center;
				NlMath::Vector3D end = normal * 10 + start;
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(start, end, glm::vec3(0.5f, 0.5f, 1.f));

				if (check == false)
				{
					return false;// no collision return false
				}

				// yes collision add to collision event and return true

				CollisionEvent newEvent;

				// get nroamel
				newEvent.collisionNormal = normal;

				// add rigid body
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;

				// set type
				newEvent.collisionType = COLRESTYPE::OBB_OBB;

				// add event
				colResolver.addCollisionEvent(newEvent);

				// return true
				return true;
				//// OBB to OBB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::AABB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to AABB

				//// OBB to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::SPHERE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to SPHERE

				//// OBB to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->colliderType == COLLIDERS::CAPSULE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to CAPSULE

				//// OBB to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
		}

		return false;
	}
}