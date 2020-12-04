#pragma once
#include "SystemCollision.h"
#include "../Core/DeltaTime.h"
#include "../Graphics/GraphicsSystem.h"//for testing, to be removed
#include "../Component/Components.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentRigidBody.h"
#include "../Input/SystemInput.h"

#include "..\Physics\SystemPhysics.h"

#include "CollisionDebugLines.h"

#undef max
#undef min
#include <algorithm> // std max

#define MESH_MAX_LOD 18
#define MESH_MIN_LOD -20

//#define ENT_TEST 1 / 0 <-- !!!
#define ENT_TEST 0


////#define USEVEL 0
////#define USEVEL 1



namespace NS_COLLISION
{
	static int test_count = 0;

	CollisionSystem::CollisionSystem() : MeshLod(0), doDrawLineMesh(false), compTtest0(nullptr), compRtest0(nullptr), entityTest0(nullptr, -1)
	{
	}
	void CollisionSystem::Load()
	{
	}
	void CollisionSystem::Init()
	{
		receiver.AttachHandler("TogglePlay", &CollisionSystem::HandleTogglePlay, this);
		colResolver.clear();

		///////////////////////////////////////////////////////////////////////////////////////////////
		//// TestMesh Key Input
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("DebugDraw", SystemInput_ns::IKEY_INSERT, "INS", SystemInput_ns::OnPress, [this]()
			{
				doDrawLineMesh = !doDrawLineMesh;
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("COLMESHLOD_INC", SystemInput_ns::IKEY_PGUP, "PGUP", SystemInput_ns::OnHold, [this]()
			{
				MeshLod++;
				if (MeshLod > MESH_MAX_LOD) MeshLod = MESH_MAX_LOD;
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("COLMESHLOD_DEC", SystemInput_ns::IKEY_PGDWN, "PGDWN", SystemInput_ns::OnHold, [this]()
			{
				MeshLod--;
				if (MeshLod < MESH_MIN_LOD) MeshLod = MESH_MIN_LOD;
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("COLMESHLOD_DEF", SystemInput_ns::IKEY_END, "END", SystemInput_ns::OnPress, [this]()
			{
				MeshLod = 0;
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("COLMESHLOD_TOG", SystemInput_ns::IKEY_HOME, "HOME", SystemInput_ns::OnPress, [this]()
			{
				doDrawLineMesh = !doDrawLineMesh;
			});

#if ENT_TEST == 1

		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("LALALA", SystemInput_ns::IKEY_M, "M", SystemInput_ns::OnPress, [this]()
			{

				Entity boxTest = G_ECMANAGER->BuildEntity(std::string("newBox").append(std::to_string(test_count)));
				++test_count;
				ComponentTransform boxTestTransform;
				boxTestTransform._position = glm::vec3(0.0f, 300.0f, 0.0f);
				boxTestTransform._scale = glm::vec3(50.0f, 50.0f, 50.0f);
				boxTest.AttachComponent<ComponentTransform>(boxTestTransform);
				//ComponentCollider boxTestCollider(COLLIDERS::AABB);
				ComponentCollider boxTestCollider(COLLIDERS::SPHERE);
				boxTest.AttachComponent<ComponentCollider>(boxTestCollider);
				ComponentRigidBody boxTestrbody;
				boxTestrbody.isStatic = false;
				boxTestrbody.isGravity = true;
				boxTestrbody.mass = 1.0f;
				boxTest.AttachComponent<ComponentRigidBody>(boxTestrbody);
			});

		//// TestMesh Key Input END
		///////////////////////////////////////////////////////////////////////////////////////////////


		//test code, to be removed
		//////////////////////////////////////////////////////////////////////////////////////
		//test creation
		//Entity cube1Test = G_ECMANAGER->BuildEntity("Test_Box1");
		//ComponentTransform Transform1;
		//ComponentRigidBody Rigid1;
		//

		//Rigid1.isStatic = true;
		//Rigid1.isGravity = true;
		//Rigid1.mass = 1.0f;

		////Transform1._rotation.x = 45;
		////Transform1._rotation.y = 40;
		////Transform1._rotation.z = 45;
		//Transform1._scale = NlMath::Vector3D(0.5f, 0.5f, 0.5f);

		//cube1Test.AttachComponent<ComponentTransform>(Transform1);
		//cube1Test.AttachComponent<ComponentRigidBody>(Rigid1);
		//


		////NS_GRAPHICS::SYS_GRAPHICS->CreateCube(cube1Test, glm::vec3(0.5f, 0.5f, 1.f));

		//Entity cube2Test = G_ECMANAGER->BuildEntity("Test_Box1");
		//ComponentTransform Transform2;
		//ComponentRigidBody Rigid2;
		//

		//Transform2._position = glm::vec3(2.5f, 0.0f, 0.f);
		////Transform2._rotation.x = -45;
		////Transform2._rotation.y = -45;
		////Transform2._rotation.z = -45;
		//Transform2._scale = NlMath::Vector3D(0.5f, 0.5f, 0.5f);


		//cube2Test.AttachComponent<ComponentTransform>(Transform2);
		//cube2Test.AttachComponent<ComponentRigidBody>(Rigid2);
		//


		//NS_GRAPHICS::SYS_GRAPHICS->CreateCube(cube2Test, glm::vec3(1.0f, 0.0f, 1.f));

		//ComponentCollider AABB1(COLLIDERS::AABB);
		//ComponentCollider AABB2(COLLIDERS::AABB);

		//ComponentCollider AABB1(COLLIDERS::OBB);
		//ComponentCollider AABB2(COLLIDERS::OBB);

		//ComponentCollider AABB1(COLLIDERS::SPHERE);
		//ComponentCollider AABB2(COLLIDERS::SPHERE);

		//ComponentCollider AABB1(COLLIDERS::CAPSULE);
		//ComponentCollider AABB2(COLLIDERS::CAPSULE);

		//AABB1.collider.aabb.colliderScale = NlMath::Vector3D(2.0f, 2.0f, 2.0f);
		//AABB2.collider.aabb.colliderScale = NlMath::Vector3D(2.0f, 2.0f, 2.0f);

		//AABB1.collider.aabb.colliderScale = NlMath::Vector3D(1.0f, 1.0f, 1.0f);
		//AABB2.collider.aabb.colliderScale = NlMath::Vector3D(1.0f, 1.0f, 1.0f);

		//cube1Test.AttachComponent<ComponentCollider>(AABB1);
		//cube2Test.AttachComponent<ComponentCollider>(AABB2);

		//ComponentCollider OBB1(COLLIDERS::OBB);
		//ComponentCollider OBB2(COLLIDERS::OBB);
		//cube1Test.AttachComponent<ComponentCollider>(OBB1);
		//cube2Test.AttachComponent<ComponentCollider>(OBB2);


		Entity platform0Test = G_ECMANAGER->BuildEntity(std::string("TestPlatform0"));
		ComponentTransform platform0Transform;
		platform0Transform._position = glm::vec3(0.0f, 100.0f, 0.0f);
		platform0Transform._scale = glm::vec3(500.0f, 50.0f, 500.0f);
		platform0Test.AttachComponent<ComponentTransform>(platform0Transform);
		ComponentCollider platform0Collider(COLLIDERS::AABB);
		platform0Collider.lod = 5;
		platform0Test.AttachComponent<ComponentCollider>(platform0Collider);
		ComponentRigidBody platform0rbody;
		platform0rbody.isStatic = true;
		platform0Test.AttachComponent<ComponentRigidBody>(platform0rbody);



		Entity boxTest = G_ECMANAGER->BuildEntity(std::string("TestBox0"));
		++test_count;
		ComponentTransform boxTestTransform;
		boxTestTransform._position = glm::vec3(0.0f, 300.0f, 0.0f);
		boxTestTransform._scale = glm::vec3(80.0f, 80.0f, 80.0f);
		boxTest.AttachComponent<ComponentTransform>(boxTestTransform);
		//ComponentCollider boxTestCollider(COLLIDERS::AABB);
		ComponentCollider boxTestCollider(COLLIDERS::SPHERE);
		boxTest.AttachComponent<ComponentCollider>(boxTestCollider);
		ComponentRigidBody boxTestrbody;
		boxTestrbody.isStatic = false;
		boxTestrbody.isGravity = true;
		boxTestrbody.mass = 1.0f;
		boxTest.AttachComponent<ComponentRigidBody>(boxTestrbody);

		ComponentTransform* compT = boxTest.getComponent<ComponentTransform>();

		ComponentRigidBody* compR = boxTest.getComponent<ComponentRigidBody>();

		compTtest0 = compT;
		compRtest0 = compR;
		entityTest0 = boxTest;


		{// ctrl

			float realDt = DELTA_T->dt / CLOCKS_PER_SEC;

#define TEST_FORCE_MAGNITUDE 1000.0f

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_RIGHT", SystemInput_ns::IKEY_D, "D", SystemInput_ns::OnHold, [this]() {
				//compT->_position.x += 1 * DELTA_T->dt / CLOCKS_PER_SEC;
				if (compRtest0->isActive)
					NS_PHYSICS::USE_THE_FORCE.addForce(entityTest0, NlMath::Vector3D(1.0f, 0.0f, 0.0f), TEST_FORCE_MAGNITUDE);
				});

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_LEFT", SystemInput_ns::IKEY_A, "A", SystemInput_ns::OnHold, [this]() {
				//compT->_position.x += -1 * DELTA_T->dt / CLOCKS_PER_SEC;
				if (compRtest0->isActive)
					NS_PHYSICS::USE_THE_FORCE.addForce(entityTest0, NlMath::Vector3D(-1.0f, 0.0f, 0.0f), TEST_FORCE_MAGNITUDE);
				});


			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_DOWN", SystemInput_ns::IKEY_S, "S", SystemInput_ns::OnHold, [this]() {
				//compT->_position.y += -1 * DELTA_T->dt / CLOCKS_PER_SEC;
				if (compRtest0->isActive)
					NS_PHYSICS::USE_THE_FORCE.addForce(entityTest0, NlMath::Vector3D(0.0f, -1.0f, 0.0f), TEST_FORCE_MAGNITUDE);
				});

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_UP", SystemInput_ns::IKEY_W, "W", SystemInput_ns::OnHold, [this]() {
				//compT->_position.y += 1 * DELTA_T->dt / CLOCKS_PER_SEC;
				if (compRtest0->isActive)
					NS_PHYSICS::USE_THE_FORCE.addForce(entityTest0, NlMath::Vector3D(0.0f, 1.0f, 0.0f), TEST_FORCE_MAGNITUDE);
				});

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_FRONT", SystemInput_ns::IKEY_Q, "Q", SystemInput_ns::OnHold, [this]() {
				//compT->_position.z += 1 * DELTA_T->dt / CLOCKS_PER_SEC;
				if (compRtest0->isActive)
					NS_PHYSICS::USE_THE_FORCE.addForce(entityTest0, NlMath::Vector3D(0.0f, 0.0f, 1.0f), TEST_FORCE_MAGNITUDE);
				});

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_BACK", SystemInput_ns::IKEY_E, "E", SystemInput_ns::OnHold, [this]() {
				//compT->_position.z += -1 * DELTA_T->dt / CLOCKS_PER_SEC;
				if (compRtest0->isActive)
					NS_PHYSICS::USE_THE_FORCE.addForce(entityTest0, NlMath::Vector3D(0.0f, 0.0f, -1.0f), TEST_FORCE_MAGNITUDE);
				});

		} // ctrl


#endif
		//////////////////////////////////////////////////////////////////////////////////////
	}

	void CollisionSystem::GameLoad()
	{
	}

	void CollisionSystem::GameInit()
	{
		// Creating an object of CSVReader
		CSVReader reader("Asset/CollisionConfig.csv");

		// Get the data from CSV File
		_colliderDataList = reader.getData();

		auto itr = G_ECMANAGER->begin<ComponentCollider>();
		auto itrEnd = G_ECMANAGER->end<ComponentCollider>();
		for (; itr != itrEnd; ++itr)
		{
			ComponentCollider* comCol = G_ECMANAGER->getComponent<ComponentCollider>(itr);
			ComponentTransform* comTrans = G_ECMANAGER->getComponent<ComponentTransform>(itr);

			UpdateCollisionBoxPos(comCol, comTrans);
		}


	}

	void CollisionSystem::FixedUpdate()
	{



		//draw debug mesh
		if (true) // lmao was this prev -> if (doDrawLineMesh) <- caused a bug xD
		{
			auto itr = G_ECMANAGER->begin<ComponentCollider>();
			auto itrEnd = G_ECMANAGER->end<ComponentCollider>();
			for (; itr != itrEnd; ++itr)
			{
				ComponentCollider* comCol = G_ECMANAGER->getComponent<ComponentCollider>(itr);
				ComponentTransform* comTrans = G_ECMANAGER->getComponent<ComponentTransform>(itr);
				UpdateCollisionBoxPos(comCol, comTrans);
				if (comCol->isCollide)
				{
					if (doDrawLineMesh)
					{
						DrawLineMesh(comTrans, comCol, MeshLod, glm::vec3(1.0f, 1.0f, 0.0f));
					}
				}
				else
				{
					if (doDrawLineMesh)
					{
						DrawLineMesh(comTrans, comCol, MeshLod);
					}
				}
				comCol->isCollide = false;
			}
		}

		if (!_isPlaying)
		{
			// Creating an object of CSVReader
			CSVReader reader("Asset/CollisionConfig.csv");

			// Get the data from CSV File
			_colliderDataList = reader.getData();
			return;
		}

		// clear collision events
		//colResolver.clear();

		auto itr1 = G_ECMANAGER->begin<ComponentCollider>();
		auto itrEnd1 = G_ECMANAGER->end<ComponentCollider>();

		auto itr2 = G_ECMANAGER->begin<ComponentCollider>();
		auto itrEnd2 = itrEnd1;

		for (; itr1 != itrEnd1; ++itr1)
		{
			for (itr2 = itr1; itr2 != itrEnd2; ++itr2)
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

				//update flags
				comCol1->prevCollisionFlag = comCol1->collisionFlag;
				comCol2->prevCollisionFlag = comCol2->collisionFlag;

				comCol1->prevTriggerFlag = comCol1->triggerFlag;
				comCol2->prevTriggerFlag = comCol2->triggerFlag;

				//Get Transforms
				ComponentTransform* comTrans1 = G_ECMANAGER->getComponent<ComponentTransform>(itr1);
				ComponentTransform* comTrans2 = G_ECMANAGER->getComponent<ComponentTransform>(itr2);

				//////////////////////////////////collision table/////////////////////////////////

				int check = _colliderDataList[comCol1->colliderTag][comCol2->colliderTag].c_str()[0] - '0';
				if (check == 0)
				{
					continue;
				}



				//////////////////////////////////////////////////////////////////////////////////
				if (comTrans1 == nullptr || comTrans2 == nullptr) continue;

				//Get rigidBody for Collision Resolution
				ComponentRigidBody* comRigid1 = G_ECMANAGER->getComponent<ComponentRigidBody>(itr1);
				ComponentRigidBody* comRigid2 = G_ECMANAGER->getComponent<ComponentRigidBody>(itr2);

				if (comRigid1 == nullptr || comRigid2 == nullptr) continue;

				if (!comRigid1->isActive || !comRigid2->isActive) continue;

				if (comRigid1->isStatic && comRigid2->isStatic) continue;
				UpdateCollisionBoxPos(comCol1, comTrans1);
				UpdateCollisionBoxPos(comCol2, comTrans2);



				if (comCol1->isCollidable == false || comCol2->isCollidable == false)
				{
					/////////////////////////check for collision trigger
					if (CheckTrigger(comCol1, comCol2))
					{
						comCol1->triggerFlag = true;
						comCol2->triggerFlag = true;
						colResolver.addTriggerEvent(Ent1, Ent2);
					}
					else
					{
						comCol1->triggerFlag = false;
						comCol2->triggerFlag = false;
					}
				}
				//check for collision, also create collision event in CheckCollision if there is collision
				else if (CheckCollision(comCol1, comCol2, comRigid1, comRigid2, comTrans1, comTrans2, Ent1, Ent2))
				{
					// store collision event
					//NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent1, glm::vec3(1.0f, 0.0f, 1.f));
					//NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent2, glm::vec3(0.8f, 0.0f, 1.f));

					comCol1->isCollide = true;
					comCol2->isCollide = true;

					/////////////////////////////////////////////////////////////////////////////////////////////
					// Draw Test mesh
					//if (doDrawLineMesh)
					//{
					//	// get trans2
					//	ComponentTransform* comTrans2 = G_ECMANAGER->getComponent<ComponentTransform>(itr2);
					//	DrawLineMesh(comTrans1, comCol1, MeshLod, glm::vec3(1.0f, 1.0f, 0.0f));
					//	DrawLineMesh(comTrans2, comCol2, MeshLod, glm::vec3(1.0f, 1.0f, 0.0f));
					//}
					// Draw Test mesh
					/////////////////////////////////////////////////////////////////////////////////////////////
				}
				else
				{
					//NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent1, glm::vec3(0.5f, 0.5f, 1.f));
					//NS_GRAPHICS::SYS_GRAPHICS->SetMeshColor(Ent2, glm::vec3(1.0f, 0.0f, 1.f));

					///////////////////////////////////////////////////////////////////////////////////////////////
					//// Draw Test mesh
					//if (doDrawLineMesh)
					//{
					//	// get trans2
					//	ComponentTransform* comTrans2 = G_ECMANAGER->getComponent<ComponentTransform>(itr2);
					//	DrawLineMesh(comTrans1, comCol1, MeshLod);
					//	DrawLineMesh(comTrans2, comCol2, MeshLod);
					//}
					// Draw Test mesh
					/////////////////////////////////////////////////////////////////////////////////////////////
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
		switch (collider->GetColliderT())
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

		switch (comCol->GetColliderT())
		{
		case COLLIDERS::PLANE:
		{
			PlaneCollider* tmp = &(comCol->collider.plane);
			tmp->colliderPosition = comCol->center;
			tmp->colliderScale = comCol->extend;
			tmp->posUpdate(comTrans);
			break;
		}
		case COLLIDERS::SPHERE:
		{
			SphereCollider* tmp = &(comCol->collider.sphere);
			tmp->colliderPosition = comCol->center;
			tmp->colliderScale = comCol->extend;
			tmp->posUpdate(comTrans);
			break;
		}
		case COLLIDERS::AABB:
		{
			AABBCollider* tmp = &(comCol->collider.aabb);
			tmp->colliderPosition = comCol->center;
			tmp->colliderScale = comCol->extend;
			tmp->posUpdate(comTrans);
			break;
		}
		case COLLIDERS::OBB:
		{
			OBBCollider* tmp = &(comCol->collider.obb);
			tmp->colliderPosition = comCol->center;
			tmp->colliderScale = comCol->extend;
			tmp->posUpdate(comTrans);
			break;
		}
		case COLLIDERS::CAPSULE:
		{
			CapsuleCollider* tmp = &(comCol->collider.capsule);
			tmp->colliderPosition = comCol->center;
			tmp->colliderScale = comCol->extend;
			tmp->posUpdate(comTrans);
			break;
		}
		default:
			throw;
			break;
		}

	}

	bool CollisionSystem::CollisionCuling(ComponentTransform* comTrans1, ComponentTransform* comTrans2)
	{
		//optimization, dont check collision if too far away
		NlMath::Vec3 distance = comTrans1->_position - comTrans2->_position;
		float sqrScale = glm::dot(comTrans1->_scale, comTrans1->_scale) + glm::dot(comTrans2->_scale, comTrans2->_scale);
		float sqrDistance = distance * distance;
		//1.5 bias
		sqrScale *= 1.5;
		return sqrDistance > sqrScale;
	}

	bool CollisionSystem::CheckTrigger(ComponentCollider* Collider1, ComponentCollider* Collider2)
	{
		/// <AABB Check Trigger>
		/// ////////////////////////////////////////////////////////////////////////////////////////////

		if (Collider1->GetColliderT() == COLLIDERS::AABB)
		{
			if (Collider2->GetColliderT() == COLLIDERS::AABB)
			{
				AABBCollider* a = &(Collider1->collider.aabb);
				AABBCollider* b = &(Collider2->collider.aabb);
				return NlMath::AABBtoAABB(*a, *b);
			}
			else if (Collider2->GetColliderT() == COLLIDERS::OBB)
			{
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
			{
				AABBCollider* a = &(Collider1->collider.aabb);
				CapsuleCollider* b = &(Collider2->collider.capsule);
				NlMath::Vec3 useless;
				if (NlMath::AABBToCapsule(*a, *b, useless) != SIDES::NO_COLLISION)
				{
					return true;
				}
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
			{
				AABBCollider* a = &(Collider1->collider.aabb);
				SphereCollider* b = &(Collider2->collider.sphere);
				NlMath::Vec3 useless;
				if (NlMath::AABB_SphereCollision(*a, *b, useless) != SIDES::NO_COLLISION)
				{
					return true;
				}
				return false;
			}
		}
		/// ////////////////////////////////////////////////////////////////////////////////////////////
		/// </AABB Check Trigger>

		/// <Sphere Check Trigger>
		/// ////////////////////////////////////////////////////////////////////////////////////////////
		if (Collider1->GetColliderT() == COLLIDERS::SPHERE)
		{
			if (Collider2->GetColliderT() == COLLIDERS::AABB)
			{
				SphereCollider* a = &(Collider1->collider.sphere);
				AABBCollider* b = &(Collider2->collider.aabb);
				NlMath::Vec3 useless;
				if (NlMath::AABB_SphereCollision(*b, *a, useless) != SIDES::NO_COLLISION)
				{
					return true;
				}
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::OBB)
			{
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
			{
				SphereCollider* a = &(Collider1->collider.sphere);
				CapsuleCollider* b = &(Collider2->collider.capsule);
				NlMath::Vec3 useless;
				return NlMath::CapsuleToSphere(*b, *a, useless);
			}
			else if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
			{
				SphereCollider* a = &(Collider1->collider.sphere);
				SphereCollider* b = &(Collider2->collider.sphere);
				NlMath::Vec3 useless;
				NlMath::Vector3D distance = NlMath::Sphere_SphereCollision(*a, *b);

				if (distance.x == 0.0f && distance.y == 0.0f && distance.z == 0.0f)
				{
					return false; // no collision return false
				}
				return true;
			}
		}
		/// ////////////////////////////////////////////////////////////////////////////////////////////
		/// </Sphere Check Trigger>

		/// <Capsule Check Trigger>
		/// ////////////////////////////////////////////////////////////////////////////////////////////

		if (Collider1->GetColliderT() == COLLIDERS::CAPSULE)
		{
			if (Collider2->GetColliderT() == COLLIDERS::AABB)
			{
				CapsuleCollider* a = &(Collider1->collider.capsule);
				AABBCollider* b = &(Collider2->collider.aabb);
				NlMath::Vec3 useless;
				if (NlMath::AABBToCapsule(*b, *a, useless) != SIDES::NO_COLLISION)
				{
					return true;
				}
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::OBB)
			{
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
			{
				CapsuleCollider* a = &(Collider1->collider.capsule);
				CapsuleCollider* b = &(Collider2->collider.capsule);
				NlMath::Vec3 useless;
				return NlMath::CapsuleToCapsule(*a, *b, useless);
			}
			else if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
			{
				CapsuleCollider* a = &(Collider1->collider.capsule);
				SphereCollider* b = &(Collider2->collider.sphere);
				NlMath::Vec3 useless;
				return NlMath::CapsuleToSphere(*a, *b, useless);
			}
		}
		/// ////////////////////////////////////////////////////////////////////////////////////////////
		/// </Capsule Check Trigger>

		if (Collider1->GetColliderT() == COLLIDERS::OBB)
		{
			if (Collider2->GetColliderT() == COLLIDERS::AABB)
			{
				OBBCollider* a = &(Collider1->collider.obb);
				AABBCollider* b = &(Collider2->collider.aabb);
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::OBB)
			{
				OBBCollider* a = &(Collider1->collider.obb);
				OBBCollider* b = &(Collider2->collider.obb);

				return NlMath::OBBToOBB(*a, *b);
			}
			else if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
			{
				OBBCollider* a = &(Collider1->collider.obb);
				CapsuleCollider* b = &(Collider2->collider.capsule);
				NlMath::Vec3 useless;
				//if (NlMath::AABBToCapsule(*a, *b, useless) != SIDES::NO_COLLISION)
				//{
				//	return true;
				//}
				return false;
			}
			else if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
			{
				OBBCollider* a = &(Collider1->collider.obb);
				SphereCollider* b = &(Collider2->collider.sphere);
				NlMath::Vec3 useless;
				//if (NlMath::AABB_SphereCollision(*a, *b, useless) != SIDES::NO_COLLISION)
				//{
				//	return true;
				//}
				return false;
			}
		}

		return false;
	}

	bool CollisionSystem::CheckCollision(
		ComponentCollider* Collider1, ComponentCollider* Collider2,
		ComponentRigidBody* Rigid1, ComponentRigidBody* Rigid2,
		ComponentTransform* Transform1, ComponentTransform* Transform2,
		Entity Entity1, Entity Entity2)
	{

		if (Collider1->GetColliderT() == COLLIDERS::AABB)
		{
			if (Collider2->GetColliderT() == COLLIDERS::AABB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// AABB to AABB
				AABBCollider* a = &(Collider1->collider.aabb);
				AABBCollider* b = &(Collider2->collider.aabb);

				CollisionEvent newEvent;
				newEvent.colidingSide = NlMath::AABB_AABBCollision(*a, *b);
				if (newEvent.colidingSide == SIDES::NO_COLLISION)
				{
					return false; // no collision return false
				}
				// yes collision add to collision event and return true
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!

				newEvent.collisionType = COLRESTYPE::AABB_AABB;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// AABB to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
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

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!

				newEvent.collisionType = COLRESTYPE::AABB_SPHERE;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// AABB to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
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

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!

				newEvent.collisionType = COLRESTYPE::AABB_CAPSULE;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// AABB to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::OBB)
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
		if (Collider1->GetColliderT() == COLLIDERS::SPHERE)
		{

			if (Collider2->GetColliderT() == COLLIDERS::AABB)
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

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!

				newEvent.collisionType = COLRESTYPE::SPHERE_AABB;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// SPHERE to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
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

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!


				// set type
				newEvent.collisionType = COLRESTYPE::SPHERE_SPHERE;

				// add event
				colResolver.addCollisionEvent(newEvent);

				// return true
				return true;
				//// SPHERE to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// SPHERE to CAPSULE
				SphereCollider* a = &Collider1->collider.sphere;
				CapsuleCollider* b = &Collider2->collider.capsule;

				// TODO !!! SPHERE to CAPSULE
				NlMath::Vector3D normal;
				bool check = NlMath::CapsuleToSphere(*b, *a, normal);

				if (check == false)
				{
					return false; // no collision return false
				}

				// yes collision add to collision event and return true

				CollisionEvent newEvent;

				// get nroamel
				newEvent.collisionNormal = -normal;

				// add rigid body
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!


				// set type
				newEvent.collisionType = COLRESTYPE::SPHERE_CAPSULE;

				// add event
				colResolver.addCollisionEvent(newEvent);

				// return true
				return true;

				//// SPHERE to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::OBB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// SPHERE to OBB

				//// SPHERE to OBB END
				///////////////////////////////////////////////////////////////////////////////////
			}
		}
		if (Collider1->GetColliderT() == COLLIDERS::CAPSULE)
		{

			if (Collider2->GetColliderT() == COLLIDERS::AABB)
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

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!

				newEvent.collisionType = COLRESTYPE::CAPSULE_AABB;
				colResolver.addCollisionEvent(newEvent);
				return true;
				//// CAPSULE to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// CAPSULE to SPHERE
				CapsuleCollider* a = &Collider1->collider.capsule;
				SphereCollider* b = &Collider2->collider.sphere;

				// TODO !!! SPHERE to CAPSULE
				NlMath::Vector3D normal;
				bool check = NlMath::CapsuleToSphere(*a, *b, normal);

				if (check == false)
				{
					return false; // no collision return false
				}

				// yes collision add to collision event and return true

				CollisionEvent newEvent;

				// get nroamel
				newEvent.collisionNormal = -normal;

				// add rigid body
				newEvent.rigid1 = Rigid1;
				newEvent.rigid2 = Rigid2;

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!


				// set type
				newEvent.collisionType = COLRESTYPE::SPHERE_CAPSULE;

				// add event
				colResolver.addCollisionEvent(newEvent);

				// return true
				return true;



				//// CAPSULE to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
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

				// !!
				newEvent.collider1 = Collider1;
				newEvent.collider2 = Collider2;
				newEvent.transform1 = Transform1;
				newEvent.transform2 = Transform2;
				newEvent.entity1 = Entity1;
				newEvent.entity2 = Entity2;
				// !!


				// set type
				newEvent.collisionType = COLRESTYPE::CAPSULE_CAPSULE;

				// add event
				colResolver.addCollisionEvent(newEvent);

				// return true
				return true;
				//// CAPSULE to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::OBB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// CAPSULE to OBB

				//// CAPSULE to OBB END
				///////////////////////////////////////////////////////////////////////////////////
			}
		}
		if (Collider1->GetColliderT() == COLLIDERS::OBB)
		{
			if (Collider2->GetColliderT() == COLLIDERS::OBB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to OBB

				OBBCollider* a = &(Collider1->collider.obb);
				OBBCollider* b = &(Collider2->collider.obb);

				Manifold tmp;

				//return NlMath::OBBToOBB(*a, *b);
				bool check = NlMath::OBB_OBBCollision(*a, *b, tmp);

				//// debug lines?
				//NlMath::Vector3D start = a->center;
				//NlMath::Vector3D end = normal  + start;

				if (doDrawLineMesh)
				{
					for (Contact& contact : tmp.contacts)
					{
						if (contact.position == NlMath::Vec3(0, 0, 0))
						{
							continue;
						}

						NS_GRAPHICS::SYS_GRAPHICS->DrawLine(contact.position, contact.position + tmp.normal * 30);
					}
				}

				CollisionEvent newEvent;
				if (check == true)
				{
					// add rigid body
					newEvent.rigid1 = Rigid1;
					newEvent.rigid2 = Rigid2;
					newEvent.collider1 = Collider1;
					newEvent.collider2 = Collider2;
					newEvent.transform1 = Transform1;
					newEvent.transform2 = Transform2;
					newEvent.entity1 = Entity1;
					newEvent.entity2 = Entity2;
					//shallow copy
					newEvent.manifold = tmp;
					// set type
					newEvent.collisionType = COLRESTYPE::OBB_OBB;
					// add event
					colResolver.addCollisionEvent(newEvent);
				}
				return check;

				//// yes collision add to collision event and return true

				//CollisionEvent newEvent;

				//// get nroamel
				//newEvent.collisionNormal = normal;

				//// add rigid body
				//newEvent.rigid1 = Rigid1;
				//newEvent.rigid2 = Rigid2;

				//// !!
				//newEvent.collider1 = Collider1;
				//newEvent.collider2 = Collider2;
				//newEvent.transform1 = Transform1;
				//newEvent.transform2 = Transform2;
				//newEvent.entity1 = Entity1;
				//newEvent.entity2 = Entity2;
				//// !!


				//// set type
				//newEvent.collisionType = COLRESTYPE::OBB_OBB;

				//// add event
				//colResolver.addCollisionEvent(newEvent);

				// return true

				//// OBB to OBB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::AABB)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to AABB

				//// OBB to AABB END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::SPHERE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to SPHERE

				//// OBB to SPHERE END
				///////////////////////////////////////////////////////////////////////////////////
			}
			if (Collider2->GetColliderT() == COLLIDERS::CAPSULE)
			{
				///////////////////////////////////////////////////////////////////////////////////
				//// OBB to CAPSULE

				//// OBB to CAPSULE END
				///////////////////////////////////////////////////////////////////////////////////
			}
		}

		return false;
	}

	void CollisionSystem::HandleTogglePlay(MessageTogglePlay& msg)
	{
		_isPlaying = msg.isPlaying;
		//if (!_isPlaying)
		//  GameExit();
	}


	void CollisionSystem::DrawLineMesh(ComponentTransform* comTrans, ComponentCollider* comCol, int lod, glm::vec3 color)
	{
		glm::vec3 colscale(
			comTrans->_scale.x * comCol->collider.aabb.colliderScale.x * 2.0f,
			comTrans->_scale.y * comCol->collider.aabb.colliderScale.y * 2.0f,
			comTrans->_scale.z * comCol->collider.aabb.colliderScale.z * 2.0f
		);

		lod += comCol->lod;

		if (lod < MESH_MIN_LOD) lod = MESH_MIN_LOD;
		if (lod > MESH_MAX_LOD) lod = MESH_MAX_LOD;
		int meshlod = lod / 2;

		NS_COLDEBUGTEST::Mesh mesh;
		switch (comCol->GetColliderT())
		{
		case COLLIDERS::AABB:
			mesh = NS_COLDEBUGTEST::CreateCube(std::max(1 + meshlod, 1), std::max(1 + meshlod, 1));
			break;
		case COLLIDERS::SPHERE:
			mesh = NS_COLDEBUGTEST::CreateSphere(std::max(8 + meshlod, 1), std::max(8 + meshlod, 1));
			break;
		case COLLIDERS::CAPSULE:
		{

			mesh = NS_COLDEBUGTEST::CreateCylinder(std::max((1 + (int)(meshlod * 0.25f)), 1), std::max(8 + meshlod, 1));
			NS_COLDEBUGTEST::Mesh mesh2;
			NS_COLDEBUGTEST::Mesh mesh3;
			mesh2 = NS_COLDEBUGTEST::CreateSphere(std::max(8 + meshlod, 1), std::max(8 + meshlod, 1));
			mesh3 = NS_COLDEBUGTEST::CreateSphere(std::max(8 + meshlod, 1), std::max(8 + meshlod, 1));

			glm::vec3 capSphereScale(
				comCol->collider.capsule.colliderScale.x,
				comCol->collider.capsule.colliderScale.x,
				comCol->collider.capsule.colliderScale.x
			);


			NS_COLDEBUGTEST::TransformMesh(
				mesh2,
				comCol->collider.capsule.tip,
				//glm::vec3(0.0f, 0.0f, 0.0f),
				comTrans->_rotation,
				capSphereScale
			);
			NS_COLDEBUGTEST::TransformMesh(
				mesh3,
				comCol->collider.capsule.base,
				//glm::vec3(0.0f, 0.0f, 0.0f), 
				comTrans->_rotation,
				capSphereScale
			);

			NS_COLDEBUGTEST::DrawMesh(mesh2, color, lod);

			NS_COLDEBUGTEST::DrawMesh(mesh3, color, lod);

		}
		break;
		case COLLIDERS::OBB:
			mesh = NS_COLDEBUGTEST::CreateCube(std::max(1 + meshlod, 1), std::max(1 + meshlod, 1));
			break;
		}

		NS_COLDEBUGTEST::TransformMesh(mesh, comCol->center + comTrans->_position, comTrans->_rotation, colscale);


		NS_COLDEBUGTEST::DrawMesh(mesh, color, lod);

	}
}