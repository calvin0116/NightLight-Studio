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
	Systems[S_PRIORITY::SP_SCENEMANAGER] = SYS_SCENE_MANAGER;
	Systems[S_PRIORITY::SP_PHYSICS] = NS_PHYSICS::SYS_PHYSICS;
	Systems[S_PRIORITY::SP_COMPONENT] = SYS_COMPONENT;
	
#ifdef _EDITOR
	Systems[S_PRIORITY::SP_EDITOR] = SYS_EDITOR;
#endif

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
	//// probably shift this to comp sys later
	{
		// factory
		ComponentManager::ComponentSetFactory comsetFac;

		auto build = [&](COMPONENTSETNAMES id)
		{
			// Building another component set
			comsetFac.StartBuild();

			comsetFac.AddComponentContainer<ComponentTransform>();
			comsetFac.AddComponentContainer<ComponentTest0>();
			comsetFac.AddComponentContainer<ComponentCollider>();
			comsetFac.AddComponentContainer<ComponentRigidBody>();
			//comsetFac.AddComponentContainer<ComponentInput>();
			//comsetFac.AddComponentContainer<ComponentLogic>();
			//comsetFac.AddComponentContainer<ComponentCamera>();
			//comsetFac.AddComponentContainer<ComponentLight>();


			// builds the component set
			ComponentManager::ComponentSet* cs = comsetFac.Build();
			// adds the component set to the component manager
			SYS_COMPONENT->AddComponentSet(id, cs);

			return cs;
		};

		// init component set manager

		build(COMPONENT_MAIN);
		build(COMPONENT_UI);

		//SYS_COMPONENT->getComponentSetMgr(COMPONENT_MAIN);
		//SYS_COMPONENT->getComponentSetMgr(COMPONENT_UI);
		//G_MAINCOMPSET = build();
		//G_UICOMPSET = build();
	}
	//// SET UP COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	// Systems OnFirst start
	for (auto my_sys : Systems)
		my_sys.second->Load();
	// Systems OnFirst start END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	//// BUILD COMPONENT SAMPLE
	{

		// WHILE OBJECTS
		// Start of creation and Entity
		Entity newEntity = G_MAINCOMPSET->BuildEntity();

		// WHILE COMPONENTS
		// Creation
		ComponentTest0 newCompComponentTest0
		{
			0,
			"Hello World",
			{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
		};
		G_MAINCOMPSET->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);
		// WHILE COMPONENTS END
		// WHILE OBJECTS END

		// add 2nd obj 
		newEntity = G_MAINCOMPSET->BuildEntity();

		ComponentTransform compT;
		compT._position.x = 1.11f;
		
		G_MAINCOMPSET->AttachComponent<ComponentTransform>(newEntity, compT);

		ComponentRigidBody compRB;
		G_MAINCOMPSET->AttachComponent<ComponentRigidBody>(newEntity, compRB);

		newCompComponentTest0.id = 1;
		G_MAINCOMPSET->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

		// add 3rd obj 
		newEntity = G_MAINCOMPSET->BuildEntity();
		G_MAINCOMPSET->AttachComponent<ComponentRigidBody>(newEntity, compRB);


		// add obj to another component set
		newCompComponentTest0.id = 999;
		newEntity = G_UICOMPSET->BuildEntity();
		G_UICOMPSET->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

		//// add to another compset
		newEntity = G_UICOMPSET->BuildEntity();
		newCompComponentTest0.id = 123;
		G_UICOMPSET->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

		///////////////////////////////
		// hierarchical entity test
		{
			std::cout << std::endl;
			std::cout << "/////////////////////////////////////" << std::endl;
			std::cout << "// hierarchical entity test - LOAD" << std::endl;
			std::cout << std::endl;

			Entity entity = G_UICOMPSET->getEntity(newEntity.getId());


			int numChild = entity.getNumChildren();
			int numDec = entity.getNumDecendants();
			int parentuid = entity.getParentId();

			std::cout << "numChild:" << numChild << std::endl;
			std::cout << "numDec:" << numDec << std::endl;
			std::cout << "parentuid:" << parentuid << std::endl;

			Entity childEntity = entity.makeChild();
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
G_UICOMPSET->AttachComponent<ComponentTest0>(childEntity, newCompComponentTest0);

std::cout << std::endl;
std::cout << "// hierarchical entity test - LOAD  END" << std::endl;
std::cout << "////////////////////////////////////" << std::endl;
std::cout << std::endl;
		}
		{
		// make 2 more childs from the entity
		Entity entity = G_UICOMPSET->getEntity(newEntity.getId());
		//entity.AttachComponent<ComponentTest0>();

		// child1
		Entity childEntity1 = entity.makeChild();
		newCompComponentTest0.id = 5432;
		G_UICOMPSET->AttachComponent<ComponentTest0>(childEntity1, newCompComponentTest0);


		// make child of child - grandchild
		Entity grandChildEntity0 = childEntity1.makeChild();
		newCompComponentTest0.id = 777;
		G_UICOMPSET->AttachComponent<ComponentTest0>(grandChildEntity0, newCompComponentTest0);


		// child2
		Entity childEntity2 = entity.makeChild();
		newCompComponentTest0.id = 543;
		G_UICOMPSET->AttachComponent<ComponentTest0>(childEntity2, newCompComponentTest0);
		}
		{
			// adding more test objects

			newEntity = G_UICOMPSET->BuildEntity();
			newCompComponentTest0.id = 1234;
			G_UICOMPSET->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

			compT._position.x = 2.22f;
			G_UICOMPSET->AttachComponent<ComponentTransform>(newEntity, compT);

			{
				// adding childs
				Entity entity = G_UICOMPSET->getEntity(newEntity.getId());

				// child1
				Entity childEntity1 = entity.makeChild();
				newCompComponentTest0.id = 7771;
				G_UICOMPSET->AttachComponent<ComponentTest0>(childEntity1, newCompComponentTest0);


				// make child of child - grandchild
				Entity grandChildEntity0 = childEntity1.makeChild();
				newCompComponentTest0.id = 8881;
				G_UICOMPSET->AttachComponent<ComponentTest0>(grandChildEntity0, newCompComponentTest0);

				// make 2nd grandchild
				Entity grandChildEntity1 = childEntity1.makeChild();
				newCompComponentTest0.id = 8882;
				G_UICOMPSET->AttachComponent<ComponentTest0>(grandChildEntity1, newCompComponentTest0);

				compT._position.x = 3.33f;
				G_UICOMPSET->AttachComponent<ComponentTransform>(grandChildEntity1, compT);

				// make 3rd grandchild
				Entity grandChildEntity2 = childEntity1.makeChild();
				newCompComponentTest0.id = 8883;
				G_UICOMPSET->AttachComponent<ComponentTest0>(grandChildEntity2, newCompComponentTest0);

				// child2
				Entity childEntity2 = entity.makeChild();
				newCompComponentTest0.id = 7772;
				G_UICOMPSET->AttachComponent<ComponentTest0>(childEntity2, newCompComponentTest0);
				

			}

			newEntity = G_UICOMPSET->BuildEntity();
			newCompComponentTest0.id = 1235;
			G_UICOMPSET->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);
		}
		// hierarchical entity test END
		///////////////////////////////
	}
	//// BUILD COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	//// GET COMPONENT SAMPLE
	//{
	//	std::cout << std::endl;
	//	std::cout << "////////////////////////////////////" << std::endl;
	//	std::cout << "// Component Sample" << std::endl;
	//	std::cout << std::endl;



	//	std::cout << std::endl;
	//	std::cout << "// Component Sample END" << std::endl;
	//	std::cout << "////////////////////////////////////" << std::endl;
	//	std::cout << std::endl;
	//}
	{
		std::cout << std::endl;
		std::cout << "////////////////////////////////////" << std::endl;
		std::cout << "// Test Components:" << std::endl;
		std::cout << std::endl;

		std::cout << std::endl;
		std::cout << "// Test Iterator:" << std::endl;
		std::cout << std::endl;

		//

		// G_UICOMPSET
        // ComponentIteratorState::ITR_BOTH   // Iterate both root and child entities // this will iterate root first then child // child is not sorted by generation, not yet anyway
        // ComponentIteratorState::ITR_ROOT   // Iterate both root only
        // ComponentIteratorState::ITR_CHILD  // Iterate both child only

		auto testItr = [&](ComponentIteratorState st)
		{
			auto itrT = G_UICOMPSET->begin<ComponentTest0>(st);
			auto itrTEnd = G_UICOMPSET->end<ComponentTest0>(st);
			while (itrT != itrTEnd)
			{
				// get the obj id
				std::cout << "Object:" << G_UICOMPSET->getObjId(itrT) << std::endl;

				// get the transform component from the iterator
				ComponentTest0* compR = G_UICOMPSET->getComponent<ComponentTest0>(itrT);
				std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

				// get another component
				ComponentTransform* compT = G_UICOMPSET->getComponent<ComponentTransform>(itrT);

				if (compT != nullptr) // nullptr -> uninitialised or deleted
					std::cout << "Transform:" << compT->_position.x << std::endl;

				// get entity
				Entity entity = G_UICOMPSET->getEntity(itrT);

				std::cout << "ID: " << entity.getId() << std::endl;
				std::cout << "Parent ID: " << entity.getParentId() << std::endl;
				std::cout << "Generation: " << entity.getGeneration() << std::endl;

				// can get component from entity too
				ComponentTransform* compT1 = entity.getComponent<ComponentTransform>();
				if (compT != compT1) throw;

				std::cout << std::endl;

				++itrT;
			}
		};
		testItr(ComponentIteratorState::ITR_BOTH);
		//testItr(ComponentIteratorState::ITR_ROOT);
		//testItr(ComponentIteratorState::ITR_CHILD);


		std::cout << std::endl;
		std::cout << "// Test Iterator END" << std::endl;
		std::cout << std::endl;


		std::cout << std::endl;
		std::cout << "// Test Get Components:" << std::endl;
		std::cout << std::endl;

		// G_MAINCOMPSET
		auto itr = G_MAINCOMPSET->begin<ComponentTest0>();
		auto itrEnd = G_MAINCOMPSET->end<ComponentTest0>();
		while (itr != itrEnd)
		{
			// get the obj id
			std::cout << "Object:" << G_MAINCOMPSET->getObjId(itr) << std::endl;

			// get the transform component from the iterator
			ComponentTest0* compR = G_MAINCOMPSET->getComponent<ComponentTest0>(itr);
			std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

			// get another component
			ComponentTransform* compT = G_MAINCOMPSET->getComponent<ComponentTransform>(itr);

			if (compT != nullptr) // nullptr -> uninitialised or deleted
				std::cout << "Transform:" << compT->_position.x << std::endl;

			std::cout << std::endl;

			++itr;
		}

		//
		int toDel0 = -1;
		int toDel1 = -1;
		int toDel2 = -1;

		auto print = [&]()
		{
			// G_UICOMPSET
			itr = G_UICOMPSET->begin<ComponentTest0>();
			itrEnd = G_UICOMPSET->end<ComponentTest0>();
			while (itr != itrEnd)
			{
				// get the obj id
				std::cout << std::endl;
				std::cout << "Object:" << G_UICOMPSET->getObjId(itr) << std::endl;

				// get the transform component from the iterator
				ComponentTest0* compR = reinterpret_cast<ComponentTest0*>(*itr);
				std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

				// get the entity from the iterator
				Entity entity = G_UICOMPSET->getEntity(itr);

				// get transform component
				ComponentTransform* compT = entity.getComponent<ComponentTransform>();
				if (compT != nullptr) // nullptr -> uninitialised or deleted
					std::cout << "Transform:" << compT->_position.x << std::endl;

				// get id for remove
				if (compR->id == 1234)
				{
					toDel1 = G_UICOMPSET->getObjId(itr);
				}

				////////////
				// childrens

				// recursive fn to do to all children

				std::function<void(ComponentManager::ChildContainerT*)> doChildrens = [&](ComponentManager::ChildContainerT* childrens)
				{
					for (int uid : *childrens)
					{
						Entity childEntity = G_UICOMPSET->getEntity(uid);


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
						if (compR->id == 5432)
						{
							toDel2 = childEntity.getId();
						}

						// get transform component
						ComponentTransform* compT = childEntity.getComponent<ComponentTransform>();
						if (compT != nullptr) // nullptr -> uninitialised or deleted
							std::cout << "Transform:" << compT->_position.x << std::endl;

						std::cout << std::endl;

						// call recursive fn for each child
						doChildrens(G_UICOMPSET->getEntity(uid).getChildren());
					}
				};

				doChildrens(entity.getChildren());

				//
				////////////

				std::cout << "Object:" << G_UICOMPSET->getObjId(itr) << " END" << std::endl;
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

		G_UICOMPSET->RemoveComponent<ComponentTransform>(toDel0);
		//print();

		G_UICOMPSET->FreeEntity(toDel2);
		//print();

		G_UICOMPSET->FreeEntity(toDel1);
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
