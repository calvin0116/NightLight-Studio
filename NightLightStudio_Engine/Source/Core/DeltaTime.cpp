
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

	dt = (float)clock() - t_start; // dt = fixed_dt
	t_start = clock();

	while (dt < fixed_dt)
	{
		dt = (float)clock() - t_start;
	}
	// fps is limited in system gfx

	//accumulatedTime += dt;

	//
	int steps = (dt / fixed_dt);
	float timeSteps = steps * fixed_dt;
	//accumulatedTime += timeSteps;
	accumulatedTime = timeSteps;

	const int maxAccFrames = 1;
	if (accumulatedTime > (fixed_dt * maxAccFrames)) // this limits accumulated frames to x frames
		accumulatedTime = (fixed_dt * maxAccFrames);

	// fml
	real_dt = dt;
	dt = fixed_dt;
	//accumulatedTime = fixed_dt;
}

void DeltaTime::end()
{
	//////
	// fps end
	//fps = 1000.0f / ((dt + _prevdt) * 0.5f);
	//fps = 1000.0f / dt;
	fps = 1000.0f / real_dt;

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
