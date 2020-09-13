#pragma once
#include "..\Vector.h"

struct COMPONENT_CONTAINERS
{
	int containerTransform;
	int containerRender;
	int containerPhysics;
	int containerRigidBody;
	int containerInput;
	int containerLogic;
	int containerCamera;
	int containerLight;
};

static COMPONENT_CONTAINERS MAINCOMPSET_CCIDS;

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
	char c;
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
