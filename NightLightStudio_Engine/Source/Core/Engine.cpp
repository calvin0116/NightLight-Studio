#include "Engine.h"

#include "SystemManager.h"

#include <time.h>
#include <iostream>


//constexpr auto MAX_DT = 15; // in ms

constexpr auto DT_SCALE = 0.001f; // 1 sec / 1000 to scale dt to ms

//constexpr auto FPS_60 = 1000.0f / 60.0f;
constexpr auto FPS_60 = 16.0f; // in ms

void FluffyUnicornEngine::Init()
{
	G_GSM.OnFirstStart();
	_engineState = ENGINE_UPDATE;
}

void FluffyUnicornEngine::Run()
{
	float fixed_dt = FPS_60;
	float dt = 0.0f;
	float accumulatedTime = 0.0f;
	int currentNumberOfSteps = 0;
	clock_t t_start = clock();
	while (_engineState)
	{

		currentNumberOfSteps = 0;

		dt = (float)clock() - t_start;
		t_start = clock();

		//while (dt < fixed_dt)
		//{
		//	dt = (float)clock() - t_start;
		//}
		// fps is limited in system gfx

		accumulatedTime += dt;

		while (accumulatedTime >= fixed_dt)
		{
			accumulatedTime -= fixed_dt;
			++currentNumberOfSteps;
		}

		// update 
		if (G_GSM.Update(fixed_dt * DT_SCALE, currentNumberOfSteps))
		{
			// returns 1
			_engineState = ENGINE_QUIT;
		}

		//////
		// fps
		float fps = 1000.0f / ((dt + _prevdt) * 0.5f);

		//std::cout << fps << std::endl;
		//std::cout << "60.00" << std::endl; // best solution for 60fps

		//_sysMgr._currentFPS = fps;

		// fps
		//////

		_prevdt = dt;
	}

	// free all memory
	//_gsm.Free();
}