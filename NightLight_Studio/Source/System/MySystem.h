#pragma once
#include "ISystem.h"
enum SYS_PIORIOTY
{
	SYS_TOOLS,
	SYS_INPUT,
	SYS_GRAPHICS,
	SYS_COLLISION,
	SYS_PHYSICS,

};

class MySystem : public ISystem
{
private:
	int id;
	SYS_PIORIOTY piority;
	

};