#pragma once
#include "..\\Math\Vector.h"

#include "ComponentManager.h"

#include "LocalVector.h"

//Component List Includes
#include "ComponentAudio.h"
#include "ComponentCollider.h"
#include "ComponentTransform.h"
#include "ComponentGraphics.h"
#include "ComponentRigidBody.h"

//extern ComponentManager::ComponentSetManager G_MAINCOMPSET;
//extern ComponentManager::ComponentSetManager G_UICOMPSET;

/*
struct IComponent
{

	virtual void Read() {};
	/*
	{
			jsonparser json;
			_position = json["postion"];
	};
	virtual void Write() {};

};*/


struct ComponentTest0
{
	struct TestVar
	{
		float f[3];
	};
	int id;
	char c[128];
	float f[16];
	LocalVector<TestVar, 4> data;
};
/*
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
*/
