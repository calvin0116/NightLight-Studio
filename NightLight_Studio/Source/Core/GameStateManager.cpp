#include "GameStateManager.h"

#include "..//Component/Components.h"

GameStateManager::GameStateManager() :
	_gamestate(GAMESTATE_UPDATE),
	_gamestateNext(GAMESTATE_UPDATE),
	_mgrCom(),
	_csmgr0()
{
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::OnFirstStart()
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP COMPONENT

	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	// init the build
	comsetFac.StartBuild();

	// adds a component using component size // adding ComponentTransform
	MAINCOMPSET_CCIDS.containerTransform = comsetFac.AddComponent(sizeof(ComponentTransform)); // save the container id

	// adds a component using settings // ComponentRender
	ComponentManager::ContainerSettings set;
	set.elementSize = sizeof(ComponentRender);
	MAINCOMPSET_CCIDS.containerRender = comsetFac.AddComponent(set); // save the container id

	// adding other components
	MAINCOMPSET_CCIDS.containerPhysics = comsetFac.AddComponent(sizeof(ComponentText));
	MAINCOMPSET_CCIDS.containerRigidBody = comsetFac.AddComponent(sizeof(ComponentRigidBody));
	MAINCOMPSET_CCIDS.containerInput = comsetFac.AddComponent(sizeof(ComponentInput));
	MAINCOMPSET_CCIDS.containerLogic = comsetFac.AddComponent(sizeof(ComponentLogic));
	MAINCOMPSET_CCIDS.containerCamera = comsetFac.AddComponent(sizeof(ComponentCamera));
	MAINCOMPSET_CCIDS.containerLight = comsetFac.AddComponent(sizeof(ComponentLight));

	// builds the component set
	ComponentManager::ComponentSet* mainComponentSet = comsetFac.Build();

	// adds the component set to the component manager
	_mgrCom.AddComponentSet(mainComponentSet);

	// init component set manager
	_csmgr0.compSet = mainComponentSet; //

	//// SET UP COMPONENT
	///////////////////////////////////////////////////////////////////////////////////////////////


	// 
	_gamestateNext = GAMESTATE_LOAD;
}

void GameStateManager::Load()
{
	// build objects from json !!! 

	void* newComponent = nullptr;

	// WHILE OBJECTS
	int newObjId = _csmgr0.BuildObject();

	// WHILE COMPONENTS
	ComponentRender newCompComponentRender
	{
		0,
		'i',
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
	};
	newComponent = &newCompComponentRender;
	_csmgr0.AttachComponent(MAINCOMPSET_CCIDS.containerTransform, newObjId, newComponent);
	// WHILE COMPONENTS END
	// WHILE OBJECTS END

	_gamestateNext = GAMESTATE_INIT;
}

void GameStateManager::Init()
{

	_gamestateNext = GAMESTATE_UPDATE;
}

int GameStateManager::Update(float dt, int step)
{
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

	return 0;
}

void GameStateManager::Free()
{
	_mgrCom.Free();
}

void GameStateManager::Unload()
{
	_mgrCom.Free();

	//_mgrCom.Unload(); // !
}
