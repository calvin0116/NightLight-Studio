#include "Engine.h"

#include "SystemManager.h"
#include "SceneManager.h"

#include "..\Component\ComponentManager.h"

#include <iostream>
#include "DeltaTime.h"


void FluffyUnicornEngine::Init(HINSTANCE& hInstance)
{
	SYS_MAN->InitSystem(hInstance);
	//_engineState = ENGINE_UPDATE;
	DELTA_T->load();
}

void FluffyUnicornEngine::Run()
{
	//Two running boolean that may need to be global depending on use case
	//bool engine_running = true;	
	bool engine_running = true;
	bool game_running = true;	//This should come from game / logic system later on

	//Engine layer
	//while (engine_running)
	//{
	
	//SYS_MAN->CombineLoad();
	//SYS_SCENEMANAGER->Load();
	
	while (engine_running)
	{
		
		//SYS_SCENEMANAGER->Init();
		SYS_MAN->LoadGame();
		//AssertsLoading?
		SYS_MAN->OnGameStart();//->CombineInit();
		//fps start
		DELTA_T->start();

		//Exit if update fails
		//Need help to remove this for messaging system
		if (!SYS_MAN->CombineUpdate())
		{
			//Temp for now
			//Any update return false will terminate the engine / game
			game_running = false;
			//scene_running = false;
			engine_running = false;
		}

		//Check for changing of scene
		if (!SYS_SCENEMANAGER->Update())
		{
			//scene_running = false;
			game_running = false;

			//If exit is being called
			/*if (SYS_SCENEMANAGER->GetToExit())
			{
				engine_running = false;
			}*/
		}
		//////
		// fps end
		DELTA_T->end();
		//std::cout << fps << std::endl;
		//std::cout << "60.00" << std::endl; // best solution for 60fps

		//_sysMgr._currentFPS = fps;
		// fps
		//////
		SYS_SCENEMANAGER->Exit();

	}

	//SYS_MAN->Unload();
	//}
	//**! Make this happen
	// free all memory
	//G_GSM.Free();
}

void FluffyUnicornEngine::Exit()
{
	SYS_MAN->Exit();
	SYS_MAN->Free();
	SYS_COMPONENT->Free();
}
