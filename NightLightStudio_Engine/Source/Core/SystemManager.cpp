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

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP WINDOW(CLIENT) INSTANCE
	NS_WINDOW::SYS_WINDOW->SetAppInstance(hInstance);

	////////////////
	// me test stuff
	std::cout << std::endl;
	std::cout << "/////////////////////////////////////" << std::endl;
	std::cout << "me test stuff:" << std::endl;
	std::cout << std::endl;
	TestVector tv;
	tv.testMyVector();
	std::cout << std::endl;
	std::cout << "me test stuff END" << std::endl;
	std::cout << "/////////////////////////////////////" << std::endl;
	std::cout << std::endl;
	// 
	////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP COMPONENT - shift this to a seperate class/function !!!

	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	auto build = [&]()
	{
		// Building another component set
		comsetFac.StartBuild();

		comsetFac.AddComponentContainer<ComponentTransform>();
		comsetFac.AddComponentContainer<ComponentRender>();
		comsetFac.AddComponentContainer<ComponentCollider>();
		comsetFac.AddComponentContainer<ComponentRigidBody>();
		comsetFac.AddComponentContainer<ComponentInput>();
		comsetFac.AddComponentContainer<ComponentLogic>();
		comsetFac.AddComponentContainer<ComponentCamera>();
		comsetFac.AddComponentContainer<ComponentLight>();

		// builds the component set
		ComponentManager::ComponentSet* cs = comsetFac.Build();
		// adds the component set to the component manager
		G_COMPMGR.AddComponentSet(cs);

		return cs;
	};

	// init component set manager
	G_MAINCOMPSET = build();
	G_UICOMPSET = build();

	//// SET UP COMPONENT
	///////////////////////////////////////////////////////////////////////////////////////////////



	//Systems OnFirst start
	for (auto my_sys : Systems)
		my_sys.second->OnFirstStart();



	///////////////////////////////////////////////////////////////////////////////////////////////
	//// LOAD COMPONENT SAMPLE


	// WHILE OBJECTS
	// Start of creation and Entity
	int newObjId = G_MAINCOMPSET.BuildObject();

	// WHILE COMPONENTS
	// Creation
	ComponentRender newCompComponentRender
	{
		0,
		"Hello World",
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
	};
	G_MAINCOMPSET.AttachComponent<ComponentRender>(newObjId, &newCompComponentRender);
	// WHILE COMPONENTS END
	// WHILE OBJECTS END

	// add 2nd obj 
	newObjId = G_MAINCOMPSET.BuildObject();

	ComponentTransform compT;
	compT._position.x = 1.11f;
	G_MAINCOMPSET.AttachComponent<ComponentTransform>(newObjId, &compT);

	newCompComponentRender.id = 1;
	G_MAINCOMPSET.AttachComponent<ComponentRender>(newObjId, &newCompComponentRender);


	// add obj to another component set
	newCompComponentRender.id = 999;
	newObjId = G_UICOMPSET.BuildObject();
	G_UICOMPSET.AttachComponent<ComponentRender>(newObjId, &newCompComponentRender);

	//// add to another compset
	newObjId = G_UICOMPSET.BuildObject();
	newCompComponentRender.id = 123;
	G_UICOMPSET.AttachComponent<ComponentRender>(newObjId, &newCompComponentRender);

	///////////////////////////////
	// hierarchical entity test
	{
		std::cout << std::endl;
		std::cout << "/////////////////////////////////////" << std::endl;
		std::cout << "hierarchical entity test - LOAD" << std::endl;
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

		newCompComponentRender.id = 54321;
		G_UICOMPSET.AttachComponent<ComponentRender>(childEntity.getId(), &newCompComponentRender);

		std::cout << std::endl;
		std::cout << "hierarchical entity test - LOAD  END" << std::endl;
		std::cout << "////////////////////////////////////" << std::endl;
		std::cout << std::endl;
	}
	{
		// make 2 more childs from the entity
		ComponentManager::ComponentSetManager::Entity entity = G_UICOMPSET.getEntity(newObjId);

		// child1
		ComponentManager::ComponentSetManager::Entity childEntity1 = entity.makeChild();
		newCompComponentRender.id = 5432;
		G_UICOMPSET.AttachComponent<ComponentRender>(childEntity1.getId(), &newCompComponentRender);


		// make child of child - grandchild
		ComponentManager::ComponentSetManager::Entity grandChildEntity0 = childEntity1.makeChild();
		newCompComponentRender.id = 777;
		G_UICOMPSET.AttachComponent<ComponentRender>(grandChildEntity0.getId(), &newCompComponentRender);


		// child2
		ComponentManager::ComponentSetManager::Entity childEntity2 = entity.makeChild();
		newCompComponentRender.id = 543;
		G_UICOMPSET.AttachComponent<ComponentRender>(childEntity2.getId(), &newCompComponentRender);
	}
	// hierarchical entity test END
	///////////////////////////////

	//// LOAD COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	//// Sample for component access

	std::cout << "Test Cmponents:" << std::endl;

	auto itr = G_MAINCOMPSET.begin<ComponentRender>();
	auto itrEnd = G_MAINCOMPSET.end<ComponentRender>();
	while (itr != itrEnd)
	{
		// get the obj id
		std::cout << "Object:" << G_MAINCOMPSET.getObjId(itr) << std::endl;

		// get the transform component from the iterator
		ComponentRender* compR = G_MAINCOMPSET.getComponent<ComponentRender>(itr);
		std::cout << "Render:" << compR->id << " " << compR->c << std::endl;

		// get another component
		ComponentTransform* compT = G_MAINCOMPSET.getComponent<ComponentTransform>(itr);
		
		if (compT != nullptr) // nullptr -> uninitialised or deleted
			std::cout << "Transform:" << compT->_position.x << std::endl;
		
		std::cout << std::endl;

		++itr;
	}


	// eg. UI can be put into a seperate component set, G_UICOMPSET
	// allows easier management of entities
	itr = G_UICOMPSET.begin<ComponentRender>();
	itrEnd = G_UICOMPSET.end<ComponentRender>();
	while (itr != itrEnd)
	{
		// get the obj id
		std::cout << "Object:" << G_UICOMPSET.getObjId(itr) << std::endl;

		// get the transform component from the iterator
		ComponentRender* compR = reinterpret_cast<ComponentRender*>(*itr);
		std::cout << "Render:" << compR->id << " " << compR->c << std::endl;

		// get the entity from the iterator
		ComponentManager::ComponentSetManager::Entity entity = G_UICOMPSET.getEntity(itr);

		// get transform component
		ComponentTransform* compT = entity.getComponent<ComponentTransform>();
		if (compT != nullptr) // nullptr -> uninitialised or deleted
			std::cout << "Transform:" << compT->_position.x << std::endl;

		////////////
		// childrens

		// recursive fn to do to all children

		std::function<void(ComponentManager::ChildContainerT*)> doChildrens = [&](ComponentManager::ChildContainerT* childrens)
		{
			for (int uid : *childrens)
			{
				std::cout << std::endl << "Print child:" << std::endl;

				ComponentManager::ComponentSetManager::Entity childEntity = G_UICOMPSET.getEntity(uid);

				std::cout << "childEntity generation:" << childEntity.getGeneration() << std::endl;

				std::cout << "childEntity id:" << childEntity.getId() << std::endl;
				std::cout << "childEntity numChild:" << childEntity.getNumChildren() << std::endl;
				std::cout << "childEntity numDec:" << childEntity.getNumDecendants() << std::endl;
				std::cout << "childEntity parentuid:" << childEntity.getParentId() << std::endl;

				compR = childEntity.getComponent<ComponentRender>();
				std::cout << "Child Render:" << compR->id << " " << compR->c << std::endl;

				std::cout << std::endl;

				// recursive
				doChildrens(G_UICOMPSET.getEntity(uid).getChildren());
			}
		};

		doChildrens(entity.getChildren());

		//
		////////////

		++itr;
	}

	std::cout << "Test Cmponents END" << std::endl;

	//// Sample for component access END
	///////////////////////////////////////////////////////////////////////////////////////////////


	//mgrComp.Free();  // testing // rmb to free !!

}