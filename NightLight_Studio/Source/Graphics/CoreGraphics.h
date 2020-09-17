#pragma once

#include "..\\Component\Components.h"


// encapsulate all graphics subsystems
class CoreGraphics
{
public:

	void AddToRenderSet(ComponentRender* comR);

	void Draw();

};