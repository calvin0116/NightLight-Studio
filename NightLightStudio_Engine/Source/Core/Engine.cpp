#include "Engine.h"

#include "SystemManager.h"

#include <time.h>
#include <iostream>

//constexpr auto MAX_DT = 15; // in ms


constexpr auto DT_SCALE = 0.001f; // 1 sec / 1000 to scale dt to ms

//constexpr auto FPS_60 = 1000.0f / 60.0f;
constexpr auto FPS_60 = 16.0f; // in ms


//Temporary placement of dt class
struct DeltaTime
{
	float fixed_dt = FPS_60;
	float dt = 0.0f;
	float _prevdt = 0.0f;

	float accumulatedTime = 0.0f;
	int currentNumberOfSteps = 0;
	clock_t t_start = 0;
	float fps = 0.0f;
	void load()
	{
		t_start = clock();
	}

	void start()
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
	}
	void end()
	{
		fps = 1000.0f / ((dt + _prevdt) * 0.5f);

		_prevdt = dt;
	}
};

// Temporary global variable for dt
DeltaTime delta_t;


void FluffyUnicornEngine::Init()
{
	SYS_MAN->OnFirstStart();
	//_engineState = ENGINE_UPDATE;
}

void FluffyUnicornEngine::Run()
{
	//Two running boolean that may need to be global depending on use case
	bool engine_running = true;	
	bool scene_running = true;
	bool game_running = true;	//This should come from game / logic system later on
	

	//Engine layer
	while (engine_running)
	{
		delta_t.load();
		SYS_MAN->CombineLoad();
		while (scene_running)
		{
			SYS_MAN->CombineInit();
			while (game_running)
			{
				//fps start
				delta_t.start();
				//Exit if update fails
				if (!SYS_MAN->CombineUpdate())
				{
					//Temp for now
					//Any update return false will terminate the engine / game
					game_running = false;
					engine_running = false;
				}
				//////
				// fps end
				delta_t.end();
				//std::cout << fps << std::endl;
				//std::cout << "60.00" << std::endl; // best solution for 60fps

				//_sysMgr._currentFPS = fps;
				// fps
				//////
			}
			SYS_MAN->Exit();
		}
		SYS_MAN->Unload();
	}
	//**! Make this happen
	// free all memory
	//G_GSM.Free();
}

void FluffyUnicornEngine::Exit()
{
	SYS_MAN->Free();
}
