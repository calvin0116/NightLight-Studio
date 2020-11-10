
#include "DeltaTime.h"
#include  <iostream>

//DeltaTime delta_t;

void DeltaTime::load()
{
	t_start = clock();
}

void DeltaTime::start()
{
	currentNumberOfSteps = 0;

	//dt = (float)clock() - t_start; // dt = fixed_dt
	t_start = clock();

	//while (dt < fixed_dt)
	//{
	//	dt = (float)clock() - t_start;
	//}
	// fps is limited in system gfx

	accumulatedTime += dt;
}

void DeltaTime::end()
{
	//////
	// fps end
	fps = 1000.0f / ((dt + _prevdt) * 0.5f);

	_prevdt = dt;
	//std::cout << fps << std::endl;
	//std::cout << "60.00" << std::endl; // best solution for 60fps

	//_sysMgr._currentFPS = fps;
	// fps
	//////
}

void DeltaTime::Exit()
{
	DestroyInstance();
}
