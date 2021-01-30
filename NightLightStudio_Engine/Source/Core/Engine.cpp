#include "Engine.h"

#include "SystemManager.h"
#include "SceneManager.h"

#include "..\Component\ComponentManager.h"
#include "Systems.h"

#include <iostream>
#include "DeltaTime.h"

#include "../Editor/LevelEditor/LevelEditor_ECHelper.h"

void FluffyUnicornEngine::Init(HINSTANCE& hInstance)
{
	//System Start Up / Load up
	SYS_MAN->StartUp(hInstance);	// Graphics / Sound Engine 
	//_engineState = ENGINE_UPDATE;
}

void FluffyUnicornEngine::Run()
{
	//=====System layer====//
    DELTA_T->load();
	//System Init
	SYS_MAN->Init();		// Master Sound Volume / Graphics settings (high res / lows) 
	while (CONFIG_DATA->GetConfigData().engineRunning)
	{
		//=====Scene Layer====//
		SYS_MAN->GameLoad();
		//Re-initial scene running when exiting from scene
		CONFIG_DATA->GetConfigData().sceneRunning = true;	
		//Aka while scene not changed
		while (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() == NS_SCENE::SC_NOCHANGE)	
		{
			SYS_MAN->GameInit();
			DELTA_T->load();
			while (CONFIG_DATA->GetConfigData().sceneRunning)	//Scene / Game loop
			{
				//fps start
				DELTA_T->start();

				//Fixed update
				int step = DeltaTime::GetInstance()->GetCurrNumberOfSteps();
				//while (DELTA_T->accumulatedTime >= DELTA_T->fixed_dt)
				for (int i = 0; i < step; ++i)
				{
					//DELTA_T->accumulatedTime -= DELTA_T->fixed_dt;
					//++DELTA_T->currentNumberOfSteps;
					SYS_MAN->FixedUpdate();
				}
				SYS_MAN->Update();		//Update

				//Check for changing of scene
				if (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() != NS_SCENE::SC_NOCHANGE)
				{
					CONFIG_DATA->GetConfigData().sceneRunning = false;
					//If exit is being called -> Change scene to Scene Exit
					if (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() == NS_SCENE::SC_EXIT)
					{
						CONFIG_DATA->GetConfigData().engineRunning = false;
					}
				}
				DELTA_T->end();
			}
		}
		SYS_MAN->GameExit();
	}

}

void FluffyUnicornEngine::Exit()
{
	//Added by Teck Wei for singleton destruction
	LE_ECHELPER->DestroyInstance();

	SYS_MAN->Free();

	DELTA_T->Exit();
	//CONFIG_DATA->Exit();
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
