#include "Engine.h"

#include "SystemManager.h"
#include "SceneManager.h"

#include "..\Component\ComponentManager.h"

#include <iostream>
#include "DeltaTime.h"


void FluffyUnicornEngine::Init(HINSTANCE& hInstance)
{
	SYS_MAN->StartUp(hInstance);	// Graphics / Sound Engine 
	//_engineState = ENGINE_UPDATE;
}

void FluffyUnicornEngine::Run()
{
	//Two running boolean that may need to be global depending on use case
	bool engine_running = true;	
	//bool scene_running = true;
	bool scene_running = true;	//This should come from game / logic system later on

	//=====System layer====//
    DELTA_T->load();
    //System Loading
    SYS_MAN->CombineLoad();		// Object Pooling / Memory allocation
	//System Init
	SYS_MAN->CombineInit();		// Master Sound Volume / Graphics settings (high res / lows) 
	while (engine_running)
	{
		//=====Scene Layer====//
		SYS_SCENE_MANAGER->LoadScene();
		while (SYS_SCENE_MANAGER->CheckChangeScene() == SC_NOCHANGE)	//Aka while scene not changed
		{
			SYS_SCENE_MANAGER->InitScene();
			while (scene_running)	//Scene / Game loop
			{
				//fps start
				DELTA_T->start();

				//Exit if update fails
				//Need help to remove this for messaging system
				if (!SYS_MAN->CombineUpdate())
				{
					//Temp for now
					//Any update return false will terminate the engine / game
					scene_running = false;
					engine_running = false;
					SYS_SCENE_MANAGER->SetNextScene(EXIT_SCENCE);
				}

				//Check for changing of scene
				if (!SYS_SCENE_MANAGER->CheckChangeScene() != SC_NOCHANGE)
				{
					scene_running = false;
					//If exit is being called
					if (SYS_SCENE_MANAGER->CheckChangeScene() == SC_EXIT)
					{
						engine_running = false;
					}
				}
				//////
				// fps end
				DELTA_T->end();
				//std::cout << fps << std::endl;
				//std::cout << "60.00" << std::endl; // best solution for 60fps

				//_sysMgr._currentFPS = fps;
				// fps
				//////
			}

			//SYS_SCENEMANAGER->Exit();
		}
		SYS_SCENE_MANAGER->ExitScene();
	}
    //SYS_MAN->Exit();
    SYS_MAN->Unload();
}

void FluffyUnicornEngine::Exit()
{
	SYS_MAN->Free();

	SYS_COMPONENT->Free();
}
