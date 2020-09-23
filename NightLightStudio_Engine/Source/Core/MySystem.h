#pragma once
#include "../../framework.h"
#include "ISystem.h"
#include "Singleton.h"

//System piority / id
enum S_PIORIOTY
{
	SP_TOOLS,
	SP_INPUT,
	SP_GRAPHICS,
	SP_COLLISION,
	SP_PHYSICS
};

class ENGINE_API MySystem : public ISystem
{
private:
	int id;
	S_PIORIOTY piority;
};