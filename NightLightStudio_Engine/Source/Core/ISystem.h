#pragma once
#include "../../framework.h"

class ISystem
{
public:
	ISystem() {};
	virtual ~ISystem() {};

	virtual void Load() {};			//First start up function for load up system with no dependancy
	virtual void Init() {};			//Initialise data with some dependancy to other system
	
	virtual void GameLoad() {};		//Loading of game assert per game / scene
	virtual void GameInit() {};		//Initialise of data for current scence

	virtual void FixedUpdate(float dt) {}; //used mostly Physics / events that needs to happen even when delta time is huge
	virtual void Update() { };

	//Unload for normal resources, Late unload depended system (e.g. graphics)
	virtual void GameExit() {};
	//virtual void LateUnload() {};

	//Final destruction for system
	virtual void Free() {};			//Resource release without dependancy
	virtual void Exit() {};			//Resource release with dependancy
};