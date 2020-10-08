#include "SystemManager.h"
#include "Systems.h"

#include "..//Component/Components.h"

#include "../IO/Json/Config.h"

#include "..//Component/Components.h"
#include "..//Component/ComponentTransform.h"
#include "..//Component/ComponentRigidBody.h"
#include "..//Component/ComponentCollider.h"

#include "../Component/LocalVector.h"

#include <functional>

#define DOTEST 1

// Do not touch
//**! Update comments please thanks
void MySystemManager::StartUp(HINSTANCE& hInstance)
{
  // === Insert your system here to get them running === //
  // === Please follow how PhysicManager is created  === // 
	//Systems[SYS_PHYSICS] = PhysicManager::GetInstance();
	Systems[S_PRIORITY::SP_WINDOW] = NS_WINDOW::SYS_WINDOW;
	Systems[S_PRIORITY::SP_GRAPHICS] = NS_GRAPHICS::SYS_GRAPHICS;
	Systems[S_PRIORITY::SP_INPUT] = SYS_INPUT;
	Systems[S_PRIORITY::SP_IO] = SYS_IO;
	Systems[S_PRIORITY::SP_AUDIO] = SYS_AUDIO;
	Systems[S_PRIORITY::SP_PHYSICS] = NS_PHYSICS::SYS_PHYSICS;
	Systems[S_PRIORITY::SP_COMPONENT] = SYS_COMPONENT;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP WINDOW(CLIENT) INSTANCE
	NS_WINDOW::SYS_WINDOW->SetAppInstance(hInstance);
	//// SET UP WINDOW(CLIENT) INSTANCE END
	///////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	// me test stuff
	{
		std::cout << std::endl;
		std::cout << "/////////////////////////////////////" << std::endl;
		std::cout << "//  me test stuff:" << std::endl;
		std::cout << std::endl;
		TestVector tv;
		tv.testMyVector();
		std::cout << std::endl;
		std::cout << "//  me test stuff END" << std::endl;
		std::cout << "/////////////////////////////////////" << std::endl;
		std::cout << std::endl;
	}
	// me test stuff END
	///////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP COMPONENT - shift this to a seperate class/function !!!
	{
		// factory
		ComponentManager::ComponentSetFactory comsetFac;

		auto build = [&]()
		{
			// Building another component set
			comsetFac.StartBuild();

			comsetFac.AddComponentContainer<ComponentTransform>();
			comsetFac.AddComponentContainer<ComponentTest0>();
			comsetFac.AddComponentContainer<ComponentCollider>();
			comsetFac.AddComponentContainer<ComponentRigidBody>();
			comsetFac.AddComponentContainer<ComponentInput>();
			comsetFac.AddComponentContainer<ComponentLogic>();
			comsetFac.AddComponentContainer<ComponentCamera>();
			comsetFac.AddComponentContainer<ComponentLight>();


			// builds the component set
			ComponentManager::ComponentSet* cs = comsetFac.Build();
			// adds the component set to the component manager
			SYS_COMPONENT->AddComponentSet(cs);

			return cs;
		};

		// init component set manager
		G_MAINCOMPSET = build();
		G_UICOMPSET = build();
	}
	//// SET UP COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	// Systems OnFirst start
	for (auto my_sys : Systems)
		my_sys.second->OnFirstStart();
	// Systems OnFirst start END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	//// BUILD COMPONENT SAMPLE
	{

		// WHILE OBJECTS
		// Start of creation and Entity
		int newObjId = G_MAINCOMPSET.BuildObject();

		// WHILE COMPONENTS
		// Creation
		ComponentTest0 newCompComponentTest0
		{
			0,
			"Hello World",
			{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
		};
		G_MAINCOMPSET.AttachComponent<ComponentTest0>(newObjId, &newCompComponentTest0);
		// WHILE COMPONENTS END
		// WHILE OBJECTS END

		// add 2nd obj 
		newObjId = G_MAINCOMPSET.BuildObject();

		ComponentTransform compT;
		compT._position.x = 1.11f;
		G_MAINCOMPSET.AttachComponent<ComponentTransform>(newObjId, &compT);

		ComponentRigidBody compRB;
		G_MAINCOMPSET.AttachComponent<ComponentRigidBody>(newObjId, &compRB);

		newCompComponentTest0.id = 1;
		G_MAINCOMPSET.AttachComponent<ComponentTest0>(newObjId, &newCompComponentTest0);

		// add 3rd obj 
		newObjId = G_MAINCOMPSET.BuildObject();
		G_MAINCOMPSET.AttachComponent<ComponentRigidBody>(newObjId, &compRB);


		// add obj to another component set
		newCompComponentTest0.id = 999;
		newObjId = G_UICOMPSET.BuildObject();
		G_UICOMPSET.AttachComponent<ComponentTest0>(newObjId, &newCompComponentTest0);

		//// add to another compset
		newObjId = G_UICOMPSET.BuildObject();
		newCompComponentTest0.id = 123;
		G_UICOMPSET.AttachComponent<ComponentTest0>(newObjId, &newCompComponentTest0);

		///////////////////////////////
		// hierarchical entity test
		{
			std::cout << std::endl;
			std::cout << "/////////////////////////////////////" << std::endl;
			std::cout << "// hierarchical entity test - LOAD" << std::endl;
			std::cout << std::endl;

			// remind self to typedef this !!
			ComponentManager::ComponentSetManager::Entity entity = G_UICOMPSET.getEntity(newObjId);


			int numChild = entity.getNumChildren();
			int numDec = entity.getNumDecendants();
			int parentuid = entity.getParentId();

			std::cout << "numChild:" << numChild << std::endl;
			std::cout << "numDec:" << numDec << std::endl;
			std::cout << "parentuid:" << parentuid << std::endl;

			ComponentManager::ComponentSetManager::Entity childEntity = entity.makeChild();
			ComponentManager::ChildContainerT* childrens = entity.getChildren();

			std::cout << std::endl;

			int numChild_child = childEntity.getNumChildren();
			int numDec_child = childEntity.getNumDecendants();
			int parentuid_child = childEntity.getParentId();

			std::cout << "numChild_child:" << childEntity.getId() << std::endl;
			std::cout << "numChild_child:" << numChild_child << std::endl;
			std::cout << "numDec_child:" << numDec_child << std::endl;
			std::cout << "parentuid_child:" << parentuid_child << std::endl;

			newCompComponentTest0.id = 54321;
			G_UICOMPSET.AttachComponent<ComponentTest0>(childEntity.getId(), &newCompComponentTest0);

			std::cout << std::endl;
			std::cout << "// hierarchical entity test - LOAD  END" << std::endl;
			std::cout << "////////////////////////////////////" << std::endl;
			std::cout << std::endl;
		}
		{
			// make 2 more childs from the entity
			ComponentManager::ComponentSetManager::Entity entity = G_UICOMPSET.getEntity(newObjId);

			// child1
			ComponentManager::ComponentSetManager::Entity childEntity1 = entity.makeChild();
			newCompComponentTest0.id = 5432;
			G_UICOMPSET.AttachComponent<ComponentTest0>(childEntity1.getId(), &newCompComponentTest0);


			// make child of child - grandchild
			ComponentManager::ComponentSetManager::Entity grandChildEntity0 = childEntity1.makeChild();
			newCompComponentTest0.id = 777;
			G_UICOMPSET.AttachComponent<ComponentTest0>(grandChildEntity0.getId(), &newCompComponentTest0);


			// child2
			ComponentManager::ComponentSetManager::Entity childEntity2 = entity.makeChild();
			newCompComponentTest0.id = 543;
			G_UICOMPSET.AttachComponent<ComponentTest0>(childEntity2.getId(), &newCompComponentTest0);
		}
		{
			// adding more test objects

			newObjId = G_UICOMPSET.BuildObject();
			newCompComponentTest0.id = 1234;
			G_UICOMPSET.AttachComponent<ComponentTest0>(newObjId, &newCompComponentTest0);

			compT._position.x = 2.22f;
			G_UICOMPSET.AttachComponent<ComponentTransform>(newObjId, &compT);

			{
				// adding childs
				ComponentManager::ComponentSetManager::Entity entity = G_UICOMPSET.getEntity(newObjId);

				// child1
				ComponentManager::ComponentSetManager::Entity childEntity1 = entity.makeChild();
				newCompComponentTest0.id = 7771;
				G_UICOMPSET.AttachComponent<ComponentTest0>(childEntity1.getId(), &newCompComponentTest0);


				// make child of child - grandchild
				ComponentManager::ComponentSetManager::Entity grandChildEntity0 = childEntity1.makeChild();
				newCompComponentTest0.id = 8881;
				G_UICOMPSET.AttachComponent<ComponentTest0>(grandChildEntity0.getId(), &newCompComponentTest0);

				// make 2nd grandchild
				ComponentManager::ComponentSetManager::Entity grandChildEntity1 = childEntity1.makeChild();
				newCompComponentTest0.id = 8882;
				G_UICOMPSET.AttachComponent<ComponentTest0>(grandChildEntity1.getId(), &newCompComponentTest0);

				compT._position.x = 3.33f;
				G_UICOMPSET.AttachComponent<ComponentTransform>(grandChildEntity1.getId(), &compT);

				// make 3rd grandchild
				ComponentManager::ComponentSetManager::Entity grandChildEntity2 = childEntity1.makeChild();
				newCompComponentTest0.id = 8883;
				G_UICOMPSET.AttachComponent<ComponentTest0>(grandChildEntity2.getId(), &newCompComponentTest0);

				// child2
				ComponentManager::ComponentSetManager::Entity childEntity2 = entity.makeChild();
				newCompComponentTest0.id = 7772;
				G_UICOMPSET.AttachComponent<ComponentTest0>(childEntity2.getId(), &newCompComponentTest0);

			}

			newObjId = G_UICOMPSET.BuildObject();
			newCompComponentTest0.id = 1235;
			G_UICOMPSET.AttachComponent<ComponentTest0>(newObjId, &newCompComponentTest0);
		}
		// hierarchical entity test END
		///////////////////////////////
	}
	//// BUILD COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	//// GET COMPONENT SAMPLE
	{
		std::cout << std::endl;
		std::cout << "////////////////////////////////////" << std::endl;
		std::cout << "// Test Components:" << std::endl;
		std::cout << std::endl;


		std::cout << std::endl;
		std::cout << "// Test Get Components:" << std::endl;
		std::cout << std::endl;

		// G_MAINCOMPSET
		auto itr = G_MAINCOMPSET.begin<ComponentTest0>();
		auto itrEnd = G_MAINCOMPSET.end<ComponentTest0>();
		while (itr != itrEnd)
		{
			// get the obj id
			std::cout << "Object:" << G_MAINCOMPSET.getObjId(itr) << std::endl;

			// get the transform component from the iterator
			ComponentTest0* compR = G_MAINCOMPSET.getComponent<ComponentTest0>(itr);
			std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

			// get another component
			ComponentTransform* compT = G_MAINCOMPSET.getComponent<ComponentTransform>(itr);

			if (compT != nullptr) // nullptr -> uninitialised or deleted
				std::cout << "Transform:" << compT->_position.x << std::endl;

			std::cout << std::endl;

			++itr;
		}

		//
		int toDel0 = -1;
		int toDel1 = -1;

		auto print = [&]()
		{
			// G_UICOMPSET
			itr = G_UICOMPSET.begin<ComponentTest0>();
			itrEnd = G_UICOMPSET.end<ComponentTest0>();
			while (itr != itrEnd)
			{
				// get the obj id
				std::cout << std::endl;
				std::cout << "Object:" << G_UICOMPSET.getObjId(itr) << std::endl;

				// get the transform component from the iterator
				ComponentTest0* compR = reinterpret_cast<ComponentTest0*>(*itr);
				std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

				// get the entity from the iterator
				ComponentManager::ComponentSetManager::Entity entity = G_UICOMPSET.getEntity(itr);

				// get transform component
				ComponentTransform* compT = entity.getComponent<ComponentTransform>();
				if (compT != nullptr) // nullptr -> uninitialised or deleted
					std::cout << "Transform:" << compT->_position.x << std::endl;

				// get id for remove
				if (compR->id == 1234)
				{
					toDel1 = G_UICOMPSET.getObjId(itr);
				}

				////////////
				// childrens

				// recursive fn to do to all children

				std::function<void(ComponentManager::ChildContainerT*)> doChildrens = [&](ComponentManager::ChildContainerT* childrens)
				{
					for (int uid : *childrens)
					{
						ComponentManager::ComponentSetManager::Entity childEntity = G_UICOMPSET.getEntity(uid);


						std::cout << std::endl << "Print child:" << std::endl;

						std::cout << "childEntity generation:" << childEntity.getGeneration() << std::endl;

						std::cout << "childEntity id:" << childEntity.getId() << std::endl;
						std::cout << "childEntity numChild:" << childEntity.getNumChildren() << std::endl;
						std::cout << "childEntity numDec:" << childEntity.getNumDecendants() << std::endl;
						std::cout << "childEntity parentuid:" << childEntity.getParentId() << std::endl;

						compR = childEntity.getComponent<ComponentTest0>();
						std::cout << "Child ComponentTest0:" << compR->id << " " << compR->c << std::endl;

						// get id for remove
						if (compR->id == 8882)
						{
							toDel0 = childEntity.getId();
						}

						// get transform component
						ComponentTransform* compT = childEntity.getComponent<ComponentTransform>();
						if (compT != nullptr) // nullptr -> uninitialised or deleted
							std::cout << "Transform:" << compT->_position.x << std::endl;

						std::cout << std::endl;

						// call recursive fn for each child
						doChildrens(G_UICOMPSET.getEntity(uid).getChildren());
					}
				};

				doChildrens(entity.getChildren());

				//
				////////////

				std::cout << "Object:" << G_UICOMPSET.getObjId(itr) << " END" << std::endl;
				std::cout << std::endl;

				++itr;
			}
		};

		print();

		std::cout << std::endl;
		std::cout << "// Test Get Components END" << std::endl;
		std::cout << std::endl;





		std::cout << std::endl;
		std::cout << "// Test Remove Components:" << std::endl;
		std::cout << std::endl;

		G_UICOMPSET.RemoveComponent<ComponentTransform>(toDel0);
		//print();

		G_UICOMPSET.FreeEntity(toDel1);
		print();

		std::cout << std::endl;
		std::cout << "// Test Remove Components END" << std::endl;
		std::cout << std::endl;

		std::cout << "// Test Components END" << std::endl;
		std::cout << "////////////////////////////////////" << std::endl;
		std::cout << std::endl;

	}
	//// GET COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////

}
