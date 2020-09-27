#pragma once
#include "..\\Math\Vector.h"
#include "ComponentManager.h"

// depreciated
//enum ContainerID{
//	containerEntity = 0, //Don't access it
//	containerRender,
//	containerPhysics,
//	containerRigidBody,
//	containerInput,
//	containerLogic,
//	containerCamera,
//	containerLight
//};

// depreciated
////Container for all intended components 
//struct ComponentsContainerID
//{
//	//Entities manager
//	ComponentManager::ComponentSetManager csmgr;
//
//	//**!Id of the container (Please change here ty)!**//
//	ComponentManager::ContainerID containerTransform ;
//	ComponentManager::ContainerID containerRender ;
//	ComponentManager::ContainerID containerPhysics;
//	ComponentManager::ContainerID containerRigidBody;
//	ComponentManager::ContainerID containerInput;
//	ComponentManager::ContainerID containerLogic;
//	ComponentManager::ContainerID containerCamera;
//	ComponentManager::ContainerID containerLight;
//};

// eg. UI can be put into a seperate component set, G_UICOMPSET
// allows easier management of entities
extern ComponentManager::ComponentSetManager G_MAINCOMPSET;
extern ComponentManager::ComponentSetManager G_UICOMPSET;


struct IComponent
{

	virtual void Read() {};
	/*
	{
			jsonparser json;
			_position = json["postion"];
	};*/
	virtual void Write() {};

};


struct ComponentRender
{
	int id;
	char c[128];
	float f[16];
};

struct ComponentInput
{
	int id;
	char c;
	float f[16];
};

struct ComponentLogic
{
	int id;
	char c;
	float f[16];
};

struct ComponentCamera
{
	int id;
	char c;
	float f[16];
};

struct ComponentLight
{
	int id;
	char c;
	float f[16];
};
