#include "SystemPhysics.h"
#include "../Component/Components.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentRigidBody.h"
#include "../Input/SystemInput.h"

namespace NS_PHYSICS
{
	PhysicsSystem::PhysicsSystem()
		:_maxspeed(100), gravity(9.8f)
	{
	}

	void PhysicsSystem::OnFirstStart()
	{
		
	}

	void PhysicsSystem::Load()
	{
	}

	void PhysicsSystem::Init()
	{
		//int newId = G_MAINCOMPSET->BuildEntity();
		//ComponentTransform compT;
		//G_MAINCOMPSET->AttachComponent<ComponentTransform>(newId, &compT);

		 /*Entity E = */
		//Entity ent = G_ECMANAGER->BuildEntity();

		//ComponentTransform c;
		//ent.AttachComponent<ComponentTransform>(ComponentTransform()); 

		//ent = G_ECMANAGER->BuildEntity().AttachComponent<ComponentTransform>(ComponentTransform());
		 // return Entity                         
		 //E.attachComp
	}

	void PhysicsSystem::FixedUpdate()
	{
		/*struct ComponentView
		{
			decltype(itr) beg;
			decltype(itr) end;
		
			auto begin() { return beg; }
			auto end() { return end; }
		} view;*/
		
		/*for (ComponentRigidBody& component : G_MAINCOMPSET->getComponentSpan<ComponentRigidBody>())
		{
		
		}*/
		auto itr = G_ECMANAGER->begin<ComponentRigidBody>();
		auto itrEnd = G_ECMANAGER->end<ComponentRigidBody>();

		for (; itr != itrEnd; ++itr)
		{
			ComponentRigidBody* compR = G_ECMANAGER->getComponent<ComponentRigidBody>(itr);
			if (compR->isStatic)
				continue;

			//itr.getComponent<>();

			//get the transform to update
			ComponentTransform* compT = G_ECMANAGER->getComponent<ComponentTransform>(itr);

			Entity ent = G_ECMANAGER->getEntity(itr);
			if (compT != ent.getComponent<ComponentTransform>())
				throw;

			//set max speed
			if (compR->velocity.x > _maxspeed.x)
				compR->velocity.x = _maxspeed.x;
			if (compR->velocity.y > _maxspeed.y)
				compR->velocity.y = _maxspeed.y;
			if (compR->velocity.z > _maxspeed.z)
				compR->velocity.z = _maxspeed.z;

			float realDt = DELTA_T->dt / CLOCKS_PER_SEC;
			compR->velocity.y -= gravity * realDt;
			
			if (compT == nullptr) // nullptr -> uninitialised or deleted
				continue;

			
			//compT->_position += (glm::vec3)compR->velocity * realDt; // I am commenting this away because the build failed // check the types

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_RIGHT", SystemInput_ns::IKEY_D, "D", SystemInput_ns::OnHold, [this]() {});
				
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_D))
			{
				
				compT->_position.x += 1 * realDt;
			}

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_LEFT", SystemInput_ns::IKEY_A, "A", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_A))
			{

				compT->_position.x -= 1 * realDt;
			}

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_DOWN", SystemInput_ns::IKEY_S, "S", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_S))
			{

				compT->_position.y -= 1 * realDt;
			}

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_UP", SystemInput_ns::IKEY_W, "W", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_W))
			{

				compT->_position.y += 1 * realDt;
			}
			
		}
		//return true;
	}

	void PhysicsSystem::Exit()
	{
		DestroyInstance();
	}

	void PhysicsSystem::Free()
	{
	}

	void PhysicsSystem::Unload()
	{
	}
}