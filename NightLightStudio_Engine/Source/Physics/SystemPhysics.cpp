#include "SystemPhysics.h"
#include "../Component/Components.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentRigidBody.h"
#include "../Input/SystemInput.h"

//#define USEVEL 0
//#define USEVEL 1


namespace NS_PHYSICS
{

	PhysicsSystem::PhysicsSystem()
		:_maxspeed(MAX_SPEED), _isPlaying(false)
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
		receiver.AttachHandler("TogglePlay", &PhysicsSystem::HandleTogglePlay, this);
	}

	void PhysicsSystem::FixedUpdate()
	{
		if (!_isPlaying)
		{
			return;
		}
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


		// update the forces
		//_forceManager.updateTranslationalForces();

		// update the forces
		_forceManager.updateTranslationalForces();


		auto itr = G_ECMANAGER->begin<ComponentRigidBody>();
		auto itrEnd = G_ECMANAGER->end<ComponentRigidBody>();

		for (; itr != itrEnd; ++itr)
		{
			
			ComponentRigidBody* compR = G_ECMANAGER->getComponent<ComponentRigidBody>(itr);
			if (compR->isStatic)
				continue;


			////////////////////////////////////////
			// TEST

			//SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_FORCE_UP", SystemInput_ns::IKEY_I, "I", SystemInput_ns::OnPress, [this]() {});

			//if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_I))
			//{
			//	_forceManager.addForce(*compR, NlMath::Vector3D(0.0f, 5.0f, 0.0f), 1.0f/*, 1000*/);
			//}

			//SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_FORCE_UP", SystemInput_ns::IKEY_I, "I", SystemInput_ns::OnHold, [this]() {});

			//if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_I))
			//{
			//	_forceManager.addForce(*compR, NlMath::Vector3D(0.0f, 5.0f, 0.0f), 1.0f);
			//}

			// TEST
			////////////////////////////////////////


			//get the transform to update
			ComponentTransform* compT = G_ECMANAGER->getComponent<ComponentTransform>(itr);

			Entity ent = G_ECMANAGER->getEntity(itr);
			if (compT != ent.getComponent<ComponentTransform>())
				throw;
			
			//set previous position
			compR->prevPos = compT->_position;

			//deltatime, convert to second
			float realDt = DELTA_T->dt / CLOCKS_PER_SEC;

			// enable gravity
			//compR->velocity.y -= gravity * realDt;
			
			if (compT == nullptr) // nullptr -> uninitialised or deleted
				continue;


			// physics
			// F = dp / dt
			// dp = F * dt
			// p = mv
			// F = d mv / dt
			// if mass is constant,
			// F = m dv/dt
			// a = dv/dt
			// F = ma
			// v = ds/dt


			// resolve forces 
			compR->force = _forceManager.resolveTranslationalForces(ent);


			// do gravity
			if (compR->isGravity)
			{
				NlMath::Vector3D gravitationalForce = compR->gravity * compR->mass;
				compR->force += gravitationalForce;
			}


			// update accleration // F = m dv/dt -> F = m a -> a = F / m
			if (compR->mass < 0.0f)
			{
				// negaitve mass
				throw;
			}
			if (compR->mass == 0.0f)
			{
				// zero mass
				compR->mass = std::numeric_limits<float>::epsilon();
			}
			compR->acceleration = compR->force / compR->mass;

			//compR->acceleration += compR->gravity; // 

			// update velocity // a = dv/dt -> dv = a * dt
			compR->velocity += compR->acceleration * realDt;
			
			// update position // v = ds/dt -> ds = v * dt

			glm::vec3 changeInDisplacement = (glm::vec3)compR->velocity* realDt;

			// limit max displacement
			if (abs(changeInDisplacement.x) > _maxspeed.x)
			{
				if(changeInDisplacement.x < 0.0f)
					changeInDisplacement.x = -_maxspeed.x;
				else
					changeInDisplacement.x = _maxspeed.x;
			}
			if (abs(changeInDisplacement.y) > _maxspeed.y)
			{
				if (changeInDisplacement.y < 0.0f)
					changeInDisplacement.y = -_maxspeed.y;
				else
					changeInDisplacement.y = _maxspeed.y;
			}
			if (abs(changeInDisplacement.z) > _maxspeed.z)
			{
				if (changeInDisplacement.z < 0.0f)
					changeInDisplacement.z = -_maxspeed.z;
				else
					changeInDisplacement.z = _maxspeed.z;
			}

			compT->_position += changeInDisplacement;

			//predict the next position for collision check // is this being used?
			compT->_nextPos = compT->_position + (glm::vec3)compR->velocity * realDt;
			
			//NLMath::Vector3d nextPosition =compT->_position = (glm::vec3)compR->velocity * realDt; // keep in rigid body




			/// <Testing>///////////////////////////////////////////////////////////////////////////////////////////////
			
//			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_RIGHT", SystemInput_ns::IKEY_D, "D", SystemInput_ns::OnHold, [this]() {});
//				
//			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_D))
//			{
//#if USEVEL == 0
//				compT->_position.x += 1 *realDt;
//#else
//				compR->velocity.x += 1 * realDt;
//#endif
//			}
//
//			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_LEFT", SystemInput_ns::IKEY_A, "A", SystemInput_ns::OnHold, [this]() {});
//
//			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_A))
//			{
//#if USEVEL == 0
//				compT->_position.x += -1 * realDt;
//#else
//				compR->velocity.x -= 1 * realDt;
//#endif
//			}
//
//			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_DOWN", SystemInput_ns::IKEY_S, "S", SystemInput_ns::OnHold, [this]() {});
//
//			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_S))
//			{
//#if USEVEL == 0
//				compT->_position.y += -1 * realDt;
//#else
//				compR->velocity.y -= 1 * realDt;
//#endif
//			}
//
//			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_UP", SystemInput_ns::IKEY_W, "W", SystemInput_ns::OnHold, [this]() {});
//
//			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_W))
//			{
//#if USEVEL == 0
//				compT->_position.y += 1 * realDt;
//#else
//				compR->velocity.y += 1 * realDt;
//#endif
//			}
//			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_FRONT", SystemInput_ns::IKEY_Q, "Q", SystemInput_ns::OnHold, [this]() {});
//
//			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_Q))
//			{
//#if USEVEL == 0
//				compT->_position.z += 1 * realDt;
//#else
//				compR->velocity.z += 1 * realDt;
//#endif
//			}
//			SYS_INPUT->GetSystemKeyPress().CreateNewEvent("OBJECT_MOVE_BACK", SystemInput_ns::IKEY_E, "E", SystemInput_ns::OnHold, [this]() {});
//
//			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_E))
//			{
//#if USEVEL == 0
//				compT->_position.z += -1 * realDt;
//#else
//				compR->velocity.z -=  1 * realDt;
//#endif
//			}



			/// </Testing>//////////////////////////////////////////////////////////////////////////////////////////
		}






		// update the forces
		_forceManager.clearTranslationalForces();


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

	void PhysicsSystem::HandleTogglePlay(MessageTogglePlay& msg)
	{
		// Handle msg here.
		std::cout << "Hello from physics!" << std::endl;
		std::cout << "TogglePlay value: " << msg.isPlaying << std::endl;

		_isPlaying = msg.isPlaying;
		//if (!_isPlaying)
		//  GameExit();
	}

}