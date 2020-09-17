

#include "GameStateManager.h"

#include "..//Component/Components.h"
#include "Systems.h"

GameStateManager::GameStateManager() :
	_gamestate(GAMESTATE_UPDATE),
	_gamestateNext(GAMESTATE_UPDATE),
	_mgrCom()
{
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::OnFirstStart()
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP COMPONENT - shift this to a seperate class/function !!!

	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	// init the build
	comsetFac.StartBuild();

	// adds a component using component size // adding ComponentTransform
	G_MAINCOMPSET_CCIDS.containerTransform = comsetFac.AddComponent(sizeof(ComponentTransform)); // save the container id

	// adds a component using settings // ComponentRender
	ComponentManager::ContainerSettings set;
	set.elementSize = sizeof(ComponentRender);
	G_MAINCOMPSET_CCIDS.containerRender = comsetFac.AddComponent(set); // save the container id

	// adding other components
	G_MAINCOMPSET_CCIDS.containerPhysics = comsetFac.AddComponent(sizeof(ComponentText));
	G_MAINCOMPSET_CCIDS.containerRigidBody = comsetFac.AddComponent(sizeof(ComponentRigidBody));
	G_MAINCOMPSET_CCIDS.containerInput = comsetFac.AddComponent(sizeof(ComponentInput));
	G_MAINCOMPSET_CCIDS.containerLogic = comsetFac.AddComponent(sizeof(ComponentLogic));
	G_MAINCOMPSET_CCIDS.containerCamera = comsetFac.AddComponent(sizeof(ComponentCamera));
	G_MAINCOMPSET_CCIDS.containerLight = comsetFac.AddComponent(sizeof(ComponentLight));

	// builds the component set
	ComponentManager::ComponentSet* mainComponentSet = comsetFac.Build();

	// adds the component set to the component manager
	_mgrCom.AddComponentSet(mainComponentSet);

	// init component set manager
	G_MAINCOMPSET_CCIDS.csmgr.compSet = mainComponentSet; //

	//// SET UP COMPONENT
	///////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.OnFirstStart();
	G_INPUT.OnFirstStart();
	G_AUDIO.OnFirstStart();
	G_LOGIC.OnFirstStart();
	G_PHYSICS.OnFirstStart();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////


	// set the next gamestate
	_gamestateNext = GAMESTATE_LOAD;
}

void GameStateManager::Load()
{
	// build objects from json !!! 
	// - shift this to a seperate class/function !!!

	void* newComponent = nullptr;

	// WHILE OBJECTS
	int newObjId = G_MAINCOMPSET_CCIDS.csmgr.BuildObject();

	// WHILE COMPONENTS
	ComponentRender newCompComponentRender
	{
		0,
		"Hello World",
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
	};
	newComponent = &newCompComponentRender;
	G_MAINCOMPSET_CCIDS.csmgr.AttachComponent(G_MAINCOMPSET_CCIDS.containerRender, newObjId, newComponent);
	// WHILE COMPONENTS END
	// WHILE OBJECTS END

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Load();
	G_INPUT.Load();
	G_AUDIO.Load();
	G_LOGIC.Load();
	G_PHYSICS.Load();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// set the next gamestate
	_gamestateNext = GAMESTATE_INIT;
}

void GameStateManager::Init()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Init();
	G_INPUT.Init();
	G_AUDIO.Init();
	G_LOGIC.Init();
	G_PHYSICS.Init();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// set the next gamestate
	_gamestateNext = GAMESTATE_UPDATE;
}

int GameStateManager::Update(float dt, int step)
{
	// set gamestate
	_gamestate = _gamestateNext;

	switch (_gamestate)
	{
	case GAMESTATE_LOAD:
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

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////


	return 0;
}

void GameStateManager::Free()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Free();
	G_INPUT.Free();
	G_AUDIO.Free();
	G_LOGIC.Free();
	G_PHYSICS.Free();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// free component memory
	_mgrCom.Free();
}

void GameStateManager::Unload()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// Systems
	G_GRAPHICS.Unload();
	G_INPUT.Unload();
	G_AUDIO.Unload();
	G_LOGIC.Unload();
	G_PHYSICS.Unload();

	//// Systems END
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// free component memory
	_mgrCom.Free();

	//_mgrCom.Unload(); // !?
}
