#pragma once
#include "..\\Math\Vector.h"

#include "ComponentManager.h"

#include "LocalVector.h"

//Component List Includes
#include "ComponentAudio.h"
#include "ComponentLoadAudio.h" // List of audio to load per scene, dunno where else to put, so creating component for it first.
#include "ComponentCollider.h"
#include "ComponentTransform.h"
#include "ComponentGraphics.h"
#include "ComponentRigidBody.h"
#include "ComponentScript.h"

#include "..\..\ISerializable.h"

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


//struct ComponentTest0 : public ISerializable
//{
//	struct TestVar
//	{
//		float f[3];
//	};
//	int id;
//	char c[128];
//	float f[16];
//	LocalVector<TestVar, 4> data;
//
//	virtual void	Read(Value& val) { UNREFERENCED_PARAMETER(val); };
//	virtual Value	Write() { return Value(); };
//	virtual Value& Write(Value& val) { return val; };
//	virtual ComponentTest0* Clone()
//	{
//		ComponentTest0* newcomp = new ComponentTest0();
//		*newcomp = *this;
//		return newcomp;
//	}
//};

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
