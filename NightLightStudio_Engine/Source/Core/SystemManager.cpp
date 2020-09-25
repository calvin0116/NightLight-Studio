

#include "SystemManager.h"
#include "Systems.h"
#include "..//Component/Components.h"

#include "..//Component/ComponentTransform.h"
#include "..//Component/ComponentPhysics.h"
/*
SystemManager G_GSM;

SystemManager::SystemManager() :
	_gamestate(GAMESTATE_UPDATE),
	_gamestateNext(GAMESTATE_UPDATE),
	_mgrCom(),
	_scnInd(0),
	_scnIndNext(0)
{
}

SystemManager::~SystemManager()
{
}
*/

// Do not touch
//**! Update comments please thanks
void MySystemManager::OnFirstStart()
{
	//Systems[SYS_PHYSICS] = PhysicManager::GetInstance();
	Systems[SP_GRAPHICS] = SYS_GRAPHIC;
	// === Insert your system here to get them running === //
	// === Please follow how PhysicManager is created  === // 
	Systems[SP_INPUT] = SYS_INPUT;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP COMPONENT - shift this to a seperate class/function !!!


	//**! Help me yong wee, not sure what to do here 
	// ok
	ComponentManager mgrComp; //<< i added this cause the next link variable is not there alr

	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	auto build = [&]()
	{
		// Building another component set
		comsetFac.StartBuild();

		comsetFac.AddComponentContainer<ComponentTransform>();
		comsetFac.AddComponentContainer<ComponentRender>();
		comsetFac.AddComponentContainer<ComponentPhysics>();
		comsetFac.AddComponentContainer<ComponentRigidBody>();
		comsetFac.AddComponentContainer<ComponentInput>();
		comsetFac.AddComponentContainer<ComponentLogic>();
		comsetFac.AddComponentContainer<ComponentCamera>();
		comsetFac.AddComponentContainer<ComponentLight>();

		// builds the component set
		ComponentManager::ComponentSet* cs = comsetFac.Build();
		// adds the component set to the component manager
		mgrComp.AddComponentSet(cs);

		return cs;
	};


	// init component set manager
	G_MAINCOMPSET = build();
	G_UICOMPSET = build();

	//// SET UP COMPONENT
	///////////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////
	////// SET UP COMPONENT - shift this to a seperate class/function !!!

	//// factory
	//ComponentManager::ComponentSetFactory comsetFac;

	//// init the build
	//comsetFac.StartBuild();

	//// adds a component using component size // adding ComponentTransform
	//G_MAINCOMPSET.containerTransform = comsetFac.AddComponentContainer(sizeof(ComponentTransform)); // save the container id

	//// adds a component using settings // ComponentRender
	//ComponentManager::ContainerSettings set;
	//set.elementSize = sizeof(ComponentRender);
	//G_MAINCOMPSET.containerRender = comsetFac.AddComponentContainer(set); // save the container id

	//// adding other components
	//G_MAINCOMPSET.containerPhysics = comsetFac.AddComponentContainer(sizeof(ComponentPhysics));
	//G_MAINCOMPSET.containerRigidBody = comsetFac.AddComponentContainer(sizeof(ComponentRigidBody));
	//G_MAINCOMPSET.containerInput = comsetFac.AddComponentContainer(sizeof(ComponentInput));
	//G_MAINCOMPSET.containerLogic = comsetFac.AddComponentContainer(sizeof(ComponentLogic));
	//G_MAINCOMPSET.containerCamera = comsetFac.AddComponentContainer(sizeof(ComponentCamera));
	//G_MAINCOMPSET.containerLight = comsetFac.AddComponentContainer(sizeof(ComponentLight));

	//// builds the component set
	//ComponentManager::ComponentSet* mainComponentSet = comsetFac.Build();
	////**! Help me yong wee, not sure what to do here 
	//ComponentManager mgrComp; //<< i added this cause the next link variable is not there alr
	//// adds the component set to the component manager
	//mgrComp.AddComponentSet(mainComponentSet);

	//// init component set manager
	//G_MAINCOMPSET.csmgr.compSet = mainComponentSet; //


	//// Building another component set
	//comsetFac.StartBuild();

	//G_UICOMPSET.containerTransform = comsetFac.AddComponentContainer<ComponentTransform>();
	//G_UICOMPSET.containerRender = comsetFac.AddComponentContainer<ComponentRender>();
	//G_UICOMPSET.containerPhysics = comsetFac.AddComponentContainer<ComponentPhysics>();
	//G_UICOMPSET.containerRigidBody = comsetFac.AddComponentContainer<ComponentRigidBody>();
	//G_UICOMPSET.containerInput = comsetFac.AddComponentContainer<ComponentInput>();
	//G_UICOMPSET.containerLogic = comsetFac.AddComponentContainer<ComponentLogic>();
	//G_UICOMPSET.containerCamera = comsetFac.AddComponentContainer<ComponentCamera>();
	//G_UICOMPSET.containerLight = comsetFac.AddComponentContainer<ComponentLight>();

	//G_UICOMPSET.csmgr = comsetFac.Build();
	//mgrComp.AddComponentSet(G_UICOMPSET.csmgr.compSet);

	////// SET UP COMPONENT
	/////////////////////////////////////////////////////////////////////////////////////////////////


	//Systems OnFirst start
	for (auto my_sys : Systems)
		my_sys.second->OnFirstStart();






	///////////////////////////////////////////////////////////////////////////////////////////////
	//// LOAD COMPONENT


	// WHILE OBJECTS
	// Start of creation and Entity
	int newObjId = G_MAINCOMPSET.BuildObject();

	// WHILE COMPONENTS
	// Creation
	// Addcomponent? > .AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);
	ComponentRender newCompComponentRender
	{
		0,
		"Hello World",
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
	};
	//G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);
	G_MAINCOMPSET.AttachComponent<ComponentRender>(newObjId, &newCompComponentRender);
	// WHILE COMPONENTS END
	// WHILE OBJECTS END

	// add 2nd obj 
	newObjId = G_MAINCOMPSET.BuildObject();

	ComponentTransform compT;
	compT._position.x = 1.11f;
	//G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerTransform, newObjId, &compT);
	G_MAINCOMPSET.AttachComponent<ComponentTransform>(newObjId, &compT);

	newCompComponentRender.id = 1;
	//G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);
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
	

	//mgrComp.Free();  // testing // rmb to free


	/*
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.OnFirstStart();
	G_INPUT.OnFirstStart();
	G_AUDIO.OnFirstStart();
	G_LOGIC.OnFirstStart();
	G_PHYSICS.OnFirstStart();
	G_IO.OnFirstStart();

	G_EDITOR.OnFirstStart();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////


	// set the next gamestate
	_gamestateNext = GAMESTATE_LOAD;
	*/
}
/*
void SystemManager::Load()
{
	Tutiorial();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Load();
	G_INPUT.Load();
	G_AUDIO.Load();
	G_LOGIC.Load();
	G_PHYSICS.Load();
	G_IO.Load();

	G_EDITOR.Load();
	
	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// set the next gamestate
	_gamestateNext = GAMESTATE_INIT;
}

void SystemManager::Init()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Init();
	G_INPUT.Init();
	G_AUDIO.Init();
	G_LOGIC.Init();
	G_PHYSICS.Init();
	G_IO.Init();

	G_EDITOR.Init();
	//G_Sys.Init
	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// set the next gamestate
	_gamestateNext = GAMESTATE_UPDATE;
}

int SystemManager::Update(float dt, int step)
{
	// set gamestate
	_gamestate = _gamestateNext;

	switch (_gamestate)
	{
	case GAMESTATE_LOAD:
		_scnInd = _scnIndNext;
		Load();
		break;
	case GAMESTATE_INIT:
		Init();
		break;
	case GAMESTATE_QUIT:
		return 1;
		break;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Update(dt);
	G_INPUT.Update(dt);
	G_AUDIO.Update(dt);
	G_LOGIC.Update(dt);
	G_PHYSICS.Update(dt);
	G_IO.Update(dt);


	G_EDITOR.Update(dt);

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////


	return 0;
}

void SystemManager::Free()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Free();
	G_INPUT.Free();
	G_AUDIO.Free();
	G_LOGIC.Free();
	G_PHYSICS.Free();
	G_IO.Free();


	G_EDITOR.Free();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// free component memory
	_mgrCom.Free();
}

void SystemManager::Unload()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Unload();
	G_INPUT.Unload();
	G_AUDIO.Unload();
	G_LOGIC.Unload();
	G_PHYSICS.Unload();
	G_IO.Unload();


	G_EDITOR.Unload();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// free component memory
	_mgrCom.Free();

	//_mgrCom.Unload(); // !?
}

int SystemManager::getScn()
{
	return _scnInd;
}

void SystemManager::setScn(int scn)
{
	_scnIndNext = scn;
}
*/

/*
void SystemManager::Tutiorial()
{
	// build objects from json !!! 
// - shift this to a seperate class/function !!!

	void* newComponent = nullptr;

	// WHILE OBJECTS
	// Start of creation and Entity
	int newObjId = G_MAINCOMPSET.csmgr.BuildObject();

	// WHILE COMPONENTS
	// Creation
	// Addcomponent? > .AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);
	ComponentRender newCompComponentRender
	{
		0,
		"Hello World",
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
	};
	newComponent = &newCompComponentRender;
	G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);
	// WHILE COMPONENTS END
	// WHILE OBJECTS END

	// add 2nd obj 
	newObjId = G_MAINCOMPSET.csmgr.BuildObject();

	ComponentTransform compT;
	compT._position._x = 1.11f;
	G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerTransform, newObjId, &compT);

	newCompComponentRender.id = 1;
	G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);


	// add obj to another component set
	newCompComponentRender.id = 999;
	newObjId = G_UICOMPSET.csmgr.BuildObject();
	G_UICOMPSET.csmgr.AttachComponent(G_UICOMPSET.containerRender, newObjId, newComponent);

}
*/
