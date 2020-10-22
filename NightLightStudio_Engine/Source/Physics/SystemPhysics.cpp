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
	}

	void PhysicsSystem::FixedUpdate()
	{
		//TA's example
		////////////////////////////////////////////////////////////////////////////////////////////////
		/*struct ComponentView
		{
			decltype(itr) beg;
			decltype(itr) end;
		
			auto begin() { return beg; }
			auto end() { return end; }
		} view;*/
		
		/*for (ComponentRigidBody& component : G_MAINCOMPSET->getComponentSpan<ComponentRigidBody>())
		{
			//itr.getComponent<>();
		}*/
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//TA's example
		auto itr = G_ECMANAGER->begin<ComponentRigidBody>();
		auto itrEnd = G_ECMANAGER->end<ComponentRigidBody>();

		for (; itr != itrEnd; ++itr)
		{
			
			ComponentRigidBody* compR = G_ECMANAGER->getComponent<ComponentRigidBody>(itr);
			if (compR->isStatic)
				continue;

			//get the transform to update
			ComponentTransform* compT = G_ECMANAGER->getComponent<ComponentTransform>(itr);

			Entity ent = G_ECMANAGER->getEntity(itr);
			if (compT != ent.getComponent<ComponentTransform>())
				throw;
			
			//set previous position
			compR->prevPos = compT->_position;

			//set max speed
			if (compR->velocity.x > _maxspeed.x)
				compR->velocity.x = _maxspeed.x;
			if (compR->velocity.y > _maxspeed.y)
				compR->velocity.y = _maxspeed.y;
			if (compR->velocity.z > _maxspeed.z)
				compR->velocity.z = _maxspeed.z;

			//deltatime, convert to second
			float realDt = DELTA_T->dt / CLOCKS_PER_SEC;

			// enable gravity
			//compR->velocity.y -= gravity * realDt;
			
			if (compT == nullptr) // nullptr -> uninitialised or deleted
				continue;


			
			compT->_position += (glm::vec3)compR->velocity * realDt;

			//predict the next position for collision check
			compT->_nextPos = compT->_position + (glm::vec3)compR->velocity * realDt;
			
			//NLMath::Vector3d nextPosition =compT->_position = (glm::vec3)compR->velocity * realDt; // keep in rigid body




			/// <Testing>///////////////////////////////////////////////////////////////////////////////////////////////
			
			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_RIGHT", SystemInput_ns::IKEY_D, "D", SystemInput_ns::OnHold, [this]() {});
				
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_D))
			{
				compR->velocity.x += 1 * realDt;
				//compT->_position.x += 1 *realDt;
				
			}

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_LEFT", SystemInput_ns::IKEY_A, "A", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_A))
			{
				compR->velocity.x -= 1 * realDt;
				//compT->_position.x += -1 * realDt;
			}

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_DOWN", SystemInput_ns::IKEY_S, "S", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_S))
			{
				compR->velocity.y -= 1 * realDt;
				//compT->_position.y += -1 * realDt;
			}

			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_UP", SystemInput_ns::IKEY_W, "W", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_W))
			{
				compR->velocity.y += 1 * realDt;
				//compT->_position.y += 1 * realDt;
			}
			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_FRONT", SystemInput_ns::IKEY_Q, "Q", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_Q))
			{
				compR->velocity.z += 1 * realDt;
				//compT->_position.z += 1 * realDt;
			}
			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_BACK", SystemInput_ns::IKEY_E, "E", SystemInput_ns::OnHold, [this]() {});

			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_E))
			{
				compR->velocity.z -=  1 * realDt;
				//compT->_position.z += -1 * realDt;
			}
			/// </Testing>//////////////////////////////////////////////////////////////////////////////////////////
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