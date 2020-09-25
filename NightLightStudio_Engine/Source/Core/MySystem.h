#pragma once
#include "../../framework.h"
#include "ISystem.h"
#include "Singleton.h"

//System piority / id
enum S_PRIORITY
{
	SP_TOOLS = 0, 
	SP_IO,
	SP_INPUT,
	SP_GRAPHICS,
	SP_COLLISION,
	SP_PHYSICS,
  SP_AUDIO
};

class ENGINE_API MySystem : public ISystem
{
private:
	int id;
	S_PRIORITY piority;
};