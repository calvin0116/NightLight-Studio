#pragma once
#include "../../framework.h"
#include "ISystem.h"
#include "Singleton.h"

//System piority / id
enum class S_PRIORITY
{

	SP_NOT_INITIALISED = -1,

	SP_TOOLS = 0,
	SP_IO,
	SP_SCENEMANAGER,
	SP_WINDOW,
	SP_INPUT,
	SP_ANIMATION,
	SP_GRAPHICS,
	SP_COLLISION,
	SP_PHYSICS,
	SP_AUDIO,
	SP_COMPONENT,
	SP_EDITOR,
	SP_LOGIC,
	SP_AI,

	SP_ASSETSMAN //<- Must be the last
};

class MySystem : public ISystem
{
protected:
	//int id;
	//S_PRIORITY priority;	//Not used
	bool isActive;
public:
	MySystem()
		:isActive{true}
		//: id{ -1 }
		//, priority{ S_PRIORITY::SP_NOT_INITIALISED }
	{};
	virtual ~MySystem() {};
};