#include "SystemManager.h"
#include "Systems.h"
#include "..//Component/Components.h"

#include "..//Component/ComponentTransform.h"
#include "..//Component/ComponentRigidBody.h"
#include "..//Component/ComponentCollider.h"

// Do not touch
//**! Update comments please thanks
void MySystemManager::OnFirstStart()
{
  // === Insert your system here to get them running === //
  // === Please follow how PhysicManager is created  === // 
	//Systems[SYS_PHYSICS] = PhysicManager::GetInstance();
	Systems[SP_GRAPHICS] = SYS_GRAPHIC;
	Systems[SP_INPUT] = SYS_INPUT;
	Systems[SP_IO] = SYS_IO;
	Systems[SP_AUDIO] = SYS_AUDIO;

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

	//// LOAD COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////


	//mgrComp.Free();  // testing // rmb to free !!

}