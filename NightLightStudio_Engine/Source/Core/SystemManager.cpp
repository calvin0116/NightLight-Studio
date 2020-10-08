#include "SystemManager.h"
#include "Systems.h"

#include "..//Component/Components.h"

#include "../IO/Json/Config.h"

#include "..//Component/Components.h"
#include "..//Component/ComponentTransform.h"
#include "..//Component/ComponentRigidBody.h"
#include "..//Component/ComponentCollider.h"


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

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP WINDOW(CLIENT) INSTANCE
	NS_WINDOW::SYS_WINDOW->SetAppInstance(hInstance);

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
	ComponentRigidBody newRigid;
	ComponentRender newCompComponentRender
	{
		0,
		"Hello World",
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
	};
	G_MAINCOMPSET.AttachComponent<ComponentRender>(newObjId, &newCompComponentRender);
	// WHILE COMPONENTS END
	// WHILE OBJECTS END
	G_MAINCOMPSET.AttachComponent<ComponentRigidBody>(newObjId, &newRigid);
	ComponentTransform compT;
	compT._position.x = 1.11f;
	G_MAINCOMPSET.AttachComponent<ComponentTransform>(newObjId, &compT);
	// add 2nd obj 
	newObjId = G_MAINCOMPSET.BuildObject();

	


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
			
		std::cout << std::endl;

		++itr;
	}

	std::cout << "Test Cmponents END" << std::endl;

	//// Sample for component access END
	///////////////////////////////////////////////////////////////////////////////////////////////


	//mgrComp.Free();  // testing // rmb to free !!

}