
#include <iostream>

#include "Component/ComponentTest.h"
#include "Core/Engine.h"

int main()
{
	ComponentTest();

	std::cout << ("test") << std::endl;



	FluffyUnicornEngine engine;

	engine.Init();

	engine.Run();

	return 0;
}

