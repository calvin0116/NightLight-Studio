#include "SystemPhysics.h"
#include "../Component/Components.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentRigidBody.h"

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
		Entity ent = G_MAINCOMPSET->BuildEntity();

		ComponentTransform c;
		ent.AttachComponent<ComponentTransform>(); 

		ent = G_MAINCOMPSET->BuildEntity().AttachComponent<ComponentTransform>();
		 // return Entity                         
		 //E.attachComp
	}

	void PhysicsSystem::Update()
	{
		auto itr = G_MAINCOMPSET->begin<ComponentRigidBody>();
		auto itrEnd = G_MAINCOMPSET->end<ComponentRigidBody>();
		
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

		for (; itr != itrEnd; ++itr)
		{
			ComponentRigidBody* compR = G_MAINCOMPSET->getComponent<ComponentRigidBody>(itr);
			if (compR->isStatic)
				continue;

			//get the transform to update
			ComponentTransform* compT = G_MAINCOMPSET->getComponent<ComponentTransform>(itr);


			//set max speed
			if (compR->velocity.x > _maxspeed.x)
				compR->velocity.x = _maxspeed.x;
			if (compR->velocity.y > _maxspeed.y)
				compR->velocity.y = _maxspeed.y;
			if (compR->velocity.z > _maxspeed.z)
				compR->velocity.z = _maxspeed.z;
			
			compR->velocity.y -= gravity;
			
			if (compT == nullptr) // nullptr -> uninitialised or deleted
				continue;

			
			//compT->_position += compR->velocity; // I am commenting this away because the build failed // check the types

			
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