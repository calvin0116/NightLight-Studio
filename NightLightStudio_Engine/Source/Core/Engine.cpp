#include "Engine.h"

#include "SystemManager.h"

#include "..\Component\ComponentManager.h"

#include <iostream>
#include "DeltaTime.h"


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

	G_COMPMGR.Free();
}
