#include "Engine.h"

#include "SystemManager.h"
#include "SceneManager.h"

#include "..\Component\ComponentManager.h"
#include "Systems.h"

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
		scene_running = true;	//Re-initial scene running when exiting from scene
		//SYS_SCENE_MANAGER->LoadScene();
		while (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() == NS_SCENE::SC_NOCHANGE)	//Aka while scene not changed
		{
			SYS_MAN->GameInit();
			//DELTA_T->end();			//To reset delta for scene change
			while (scene_running)	//Scene / Game loop
			{
				//fps start
				DELTA_T->start();

				//Exit if update fails
				//Fixed update
				while (DELTA_T->accumulatedTime >= DELTA_T->fixed_dt)
				{
					DELTA_T->accumulatedTime -= DELTA_T->fixed_dt;
					++DELTA_T->currentNumberOfSteps;
					SYS_MAN->FixedUpdate();
				}
				SYS_MAN->Update();

				//Check for changing of scene
				if (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() != NS_SCENE::SC_NOCHANGE)
				{
					scene_running = false;
					//If exit is being called -> Change scene to Scene Exit
					if (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() == NS_SCENE::SC_EXIT)
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

}

void FluffyUnicornEngine::Exit()
{
	SYS_MAN->Free();

	DELTA_T->Exit();
	CONFIG_DATA->Exit();
	//SYS_MAN->Free();
	SYS_COMPONENT->Free();
	
	SYS_MAN->Exit();

	// ==== Manaul singleton deletion (debugging purpose)========//
	/*
	NS_WINDOW::SYS_WINDOW->DestroyInstance();
	NS_GRAPHICS::SYS_GRAPHICS->DestroyInstance();
	SYS_INPUT->DestroyInstance();
	SYS_IO->DestroyInstance();
	SYS_AUDIO->DestroyInstance();
	NS_SCENE::SYS_SCENE_MANAGER->DestroyInstance();
	NS_PHYSICS::SYS_PHYSICS->DestroyInstance();
	SYS_COMPONENT->DestroyInstance();
	NS_COLLISION::SYS_COLLISION->DestroyInstance();
	*/
	
}
