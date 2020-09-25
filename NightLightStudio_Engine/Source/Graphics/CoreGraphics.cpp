#include "CoreGraphics.h"

#include <iostream>

void CoreGraphics::AddToRenderSet(ComponentRender*)
{

}

void CoreGraphics::Draw()
{
	std::cout << "CoreGraphics.Draw()" << std::endl;
}
