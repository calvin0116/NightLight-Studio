

#include "SystemManager.h"

#include "..//Component/Components.h"
#include "Systems.h"

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

void SystemManager::OnFirstStart()
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP COMPONENT - shift this to a seperate class/function !!!

	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	// init the build
	comsetFac.StartBuild();

	// adds a component using component size // adding ComponentTransform
	G_MAINCOMPSET.containerTransform = comsetFac.AddComponent(sizeof(ComponentTransform)); // save the container id

	// adds a component using settings // ComponentRender
	ComponentManager::ContainerSettings set;
	set.elementSize = sizeof(ComponentRender);
	G_MAINCOMPSET.containerRender = comsetFac.AddComponent(set); // save the container id

	// adding other components
	G_MAINCOMPSET.containerPhysics = comsetFac.AddComponent(sizeof(ComponentPhysics));
	G_MAINCOMPSET.containerRigidBody = comsetFac.AddComponent(sizeof(ComponentRigidBody));
	G_MAINCOMPSET.containerInput = comsetFac.AddComponent(sizeof(ComponentInput));
	G_MAINCOMPSET.containerLogic = comsetFac.AddComponent(sizeof(ComponentLogic));
	G_MAINCOMPSET.containerCamera = comsetFac.AddComponent(sizeof(ComponentCamera));
	G_MAINCOMPSET.containerLight = comsetFac.AddComponent(sizeof(ComponentLight));

	// builds the component set
	ComponentManager::ComponentSet* mainComponentSet = comsetFac.Build();

	// adds the component set to the component manager
	_mgrCom.AddComponentSet(mainComponentSet);

	// init component set manager
	G_MAINCOMPSET.csmgr.compSet = mainComponentSet; //


	// Building another component set
	comsetFac.StartBuild();
	G_UICOMPSET.containerTransform = comsetFac.AddComponent(sizeof(ComponentTransform)); // save the container id
	G_UICOMPSET.containerRender = comsetFac.AddComponent(sizeof(ComponentRender));
	G_UICOMPSET.containerPhysics = comsetFac.AddComponent(sizeof(ComponentPhysics));
	G_UICOMPSET.containerRigidBody = comsetFac.AddComponent(sizeof(ComponentRigidBody));
	G_UICOMPSET.containerInput = comsetFac.AddComponent(sizeof(ComponentInput));
	G_UICOMPSET.containerLogic = comsetFac.AddComponent(sizeof(ComponentLogic));
	G_UICOMPSET.containerCamera = comsetFac.AddComponent(sizeof(ComponentCamera));
	G_UICOMPSET.containerLight = comsetFac.AddComponent(sizeof(ComponentLight));
	G_UICOMPSET.csmgr = comsetFac.Build();
	_mgrCom.AddComponentSet(G_UICOMPSET.csmgr.compSet);


	//// SET UP COMPONENT
	///////////////////////////////////////////////////////////////////////////////////////////////


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
}

void SystemManager::Load()
{


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
