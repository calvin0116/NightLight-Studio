#include "Engine.h"

#include "SystemManager.h"
#include "SceneManager.h"

#include "..\Component\ComponentManager.h"

#include <iostream>
#include "DeltaTime.h"


void FluffyUnicornEngine::Init(HINSTANCE& hInstance)
{
	//System Start Up / Load up
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
	//System Init
	SYS_MAN->Init();		// Master Sound Volume / Graphics settings (high res / lows) 
	while (engine_running)
	{
		//=====Scene Layer====//
		SYS_MAN->GameLoad();
		//SYS_SCENE_MANAGER->LoadScene();
		while (SYS_SCENE_MANAGER->CheckChangeScene() == SC_NOCHANGE)	//Aka while scene not changed
		{
			SYS_MAN->GameInit();
			//SYS_SCENE_MANAGER->InitScene();
			while (scene_running)	//Scene / Game loop
			{
				//fps start
				DELTA_T->start();

				//Exit if update fails
				//Need help to remove this for messaging system

				SYS_MAN->FixedUpdate();			
				SYS_MAN->Update();

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
		SYS_MAN->GameExit();
		//SYS_SCENE_MANAGER->ExitScene();
	}
    SYS_MAN->Free();
}

void FluffyUnicornEngine::Exit()
{
	//SYS_MAN->Free();
	SYS_MAN->Exit();
	SYS_COMPONENT->Free();
}
