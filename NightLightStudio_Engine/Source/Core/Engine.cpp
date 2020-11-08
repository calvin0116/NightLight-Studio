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

	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TogglePlay", SystemInput_ns::IKEY_NUMPAD_8, "TogglePlay", SystemInput_ns::OnRelease, [this]()
	{
		if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IKEY_NUMPAD_8))
		{
			CONFIG_DATA->GetConfigData().sceneRunning = !CONFIG_DATA->GetConfigData().sceneRunning;
			std::cout << "Scene Running: " << CONFIG_DATA->GetConfigData().sceneRunning << std::endl;
			if (CONFIG_DATA->GetConfigData().sceneRunning)
				gameState = GameState::Load;
			else
				gameState = GameState::Exit;
		}
	});
}

void FluffyUnicornEngine::Run()
{
	//Two running boolean that may need to be global depending on use case
	gameState = GameState::Load;

	//=====System layer====//
  DELTA_T->load();
	//System Init
	SYS_MAN->Init();		// Master Sound Volume / Graphics settings (high res / lows) 
	while (CONFIG_DATA->GetConfigData().engineRunning)
	{
		DELTA_T->start();
		switch (gameState)
		{
		case GameState::Load:
			SYS_MAN->GameLoad();
			gameState = GameState::Init;
			break;
		case GameState::Init:
			SYS_MAN->GameInit();
			gameState = GameState::Update;
			break;
		case GameState::Update:
			while (DELTA_T->accumulatedTime >= DELTA_T->fixed_dt)
			{
				DELTA_T->accumulatedTime -= DELTA_T->fixed_dt;
				++DELTA_T->currentNumberOfSteps;
				SYS_MAN->FixedUpdate(CONFIG_DATA->GetConfigData().sceneRunning);
			}
			SYS_MAN->Update(CONFIG_DATA->GetConfigData().sceneRunning);
			break;
		case GameState::Exit:
			SYS_MAN->GameExit();
			CONFIG_DATA->GetConfigData().sceneRunning = false;
			break;
		default:
			break;
		}
		// Check for scene changes
		NS_SCENE::SCENE_CHANGE sceneState = NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene();
		switch (sceneState)
		{
		case NS_SCENE::SC_CHANGE:
			SYS_MAN->GameExit();
			gameState = GameState::Load;
			break;
		case NS_SCENE::SC_RESTART:
			gameState = GameState::Init;
			break;
		case NS_SCENE::SC_EXIT:
			if (CONFIG_DATA->GetConfigData().sceneRunning)
				SYS_MAN->GameExit();
			CONFIG_DATA->GetConfigData().engineRunning = false;
			break;
		default:
			break;
		}
		DELTA_T->end();

		//SYS_MAN->Update(scene_running);

		////=====Scene Layer====//
		//SYS_MAN->GameLoad();
		//scene_running = true;	//Re-initial scene running when exiting from scene
		////SYS_SCENE_MANAGER->LoadScene();
		//while (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() == NS_SCENE::SC_NOCHANGE)	//Aka while scene not changed
		//{
		//	SYS_MAN->GameInit();
		//	//DELTA_T->end();			//To reset delta for scene change
		//	while (scene_running)	//Scene / Game loop
		//	{
		//		//fps start
		//		DELTA_T->start();

		//		//Exit if update fails
		//		//Fixed update
		//		while (DELTA_T->accumulatedTime >= DELTA_T->fixed_dt)
		//		{
		//			DELTA_T->accumulatedTime -= DELTA_T->fixed_dt;
		//			++DELTA_T->currentNumberOfSteps;
		//			SYS_MAN->FixedUpdate();
		//		}
		//		SYS_MAN->Update();

		//		//Check for changing of scene
		//		if (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() != NS_SCENE::SC_NOCHANGE)
		//		{
		//			scene_running = false;
		//			//If exit is being called -> Change scene to Scene Exit
		//			if (NS_SCENE::SYS_SCENE_MANAGER->CheckChangeScene() == NS_SCENE::SC_EXIT)
		//			{
		//				engine_running = false;
		//			}
		//		}
		//		//////
		//		// fps end
		//		DELTA_T->end();
		//		//std::cout << fps << std::endl;
		//		//std::cout << "60.00" << std::endl; // best solution for 60fps

		//		//_sysMgr._currentFPS = fps;
		//		// fps
		//		//////
		//	}

		//	//SYS_SCENEMANAGER->Exit();
		//}
		//SYS_MAN->GameExit();
		////SYS_SCENE_MANAGER->ExitScene();
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
