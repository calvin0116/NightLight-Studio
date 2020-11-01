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

//For json serialising
#include "..\..\ISerializable.h"

//~~! Notes for adding component~~// < By Teck Wei, More info can pm me
//When a new component is created, these are places you need at your component to make them
//appear in the game

//<OwnComponent>.cpp
// Inherit ISeriabllzable
// Override the Write and Read
//*Example is shown in TransformComponent

//*SceneManager.cpp
//~~ void ComponentsCreation(Value& Comp_list, Entity& entity)
//~~~~~ ^This function to read in json data of your component and create them

//*LevelEditor_Inspector.cpp
//~~ void InspectorWindow::Run() < Most likely gonna put the component parts into another function
//~~~~~^ This function is where you insert your component into the window to inspector window
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


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

	virtual Value	Write() { return Value(rapidjson::kObjectType); };
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
