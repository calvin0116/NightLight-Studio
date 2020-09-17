#pragma once
#include "..\Vector.h"
#include "ComponentManager.h"

struct COMPONENT_CONTAINERS
{
	ComponentManager::ComponentSetManager csmgr;
	ComponentManager::ContainerID containerTransform;
	ComponentManager::ContainerID containerRender;
	ComponentManager::ContainerID containerPhysics;
	ComponentManager::ContainerID containerRigidBody;
	ComponentManager::ContainerID containerInput;
	ComponentManager::ContainerID containerLogic;
	ComponentManager::ContainerID containerCamera;
	ComponentManager::ContainerID containerLight;
};

extern COMPONENT_CONTAINERS G_MAINCOMPSET_CCIDS;


struct ComponentTransform
{
	NlMath::Vector3D _position;
	NlMath::Vector3D _rotation;
	NlMath::Vector3D _scale;

	ComponentTransform();
	~ComponentTransform();

	//To add other stuff
};

struct ComponentRender
{
	int id;
	char c[128];
	float f[16];
};

struct ComponentText
{
	int id;
	char c;
	float f[16];
};

struct ComponentRigidBody
{
	int id;
	char c;
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
