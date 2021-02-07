#include "SystemManager.h"
#include "Systems.h"


#include "../IO/Json/Config.h"

#include <functional>

#include "..//Component/Components.h"
#include "../Messaging/SystemBroadcaster.h"


#define DOTEST 1

// Do not touch
//**! Update comments please thanks
void MySystemManager::StartUp(HINSTANCE& hInstance)
{
  // === Insert your system here to get them running === //
  // === Please follow how PhysicManager is created  === // 
	//Systems[SYS_PHYSICS] = PhysicManager::GetInstance();
	
	Systems[S_PRIORITY::SP_WINDOW] = NS_WINDOW::SYS_WINDOW;
	Systems[S_PRIORITY::SP_GRAPHICS] = NS_GRAPHICS::SYS_GRAPHICS;
	Systems[S_PRIORITY::SP_INPUT] = SYS_INPUT;
	Systems[S_PRIORITY::SP_IO] = SYS_IO;
	Systems[S_PRIORITY::SP_AUDIO] = SYS_AUDIO;
	Systems[S_PRIORITY::SP_SCENEMANAGER] = NS_SCENE::SYS_SCENE_MANAGER;
	Systems[S_PRIORITY::SP_PHYSICS] = NS_PHYSICS::SYS_PHYSICS;
	Systems[S_PRIORITY::SP_COMPONENT] = SYS_COMPONENT;
	Systems[S_PRIORITY::SP_COLLISION] = NS_COLLISION::SYS_COLLISION;
	Systems[S_PRIORITY::SP_LOGIC] = NS_LOGIC::SYS_LOGIC;
	Systems[S_PRIORITY::SP_AI] = NS_AI::SYS_AI;
	
#ifdef _EDITOR
	Systems[S_PRIORITY::SP_EDITOR] = SYS_EDITOR;
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP WINDOW(CLIENT) INSTANCE
	NS_WINDOW::SYS_WINDOW->SetAppInstance(hInstance);
	//// SET UP WINDOW(CLIENT) INSTANCE END
	///////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//// Add components in fn
	SYS_COMPONENT->ComponentCreation();
	//// 
	/////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Systems OnFirst start
	for (auto my_sys : Systems)
		my_sys.second->Load();
	// Systems OnFirst start END
	///////////////////////////////////////////////////////////////////////////////////////////////



}


//== Game / Usage of assert phase
//ENGINE_API void EarlyInit() { for (auto my_sys : Systems) my_sys.second->EarlyInit(); };

inline ENGINE_API void MySystemManager::GameInit() {
	for (auto my_sys : Systems) 
		my_sys.second->GameInit();

#ifndef _EDITOR
	MessageTogglePlay isPlaying(true, "TogglePlay");
	MessageTogglePlay isPlaying_1(true, "BeforePlay");
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying_1);
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying);
#endif
}

void MySystemManager::FixedUpdate(float dt)
{
#ifdef _EDITOR
	int i = 0;
#endif

	for (auto my_sys : Systems)
	{
#ifdef _EDITOR
		std::vector<float>* sysUsage = SYS_EDITOR->GetSystemsUsage();

		LARGE_INTEGER start{}, end{}, elapsed{};
		LARGE_INTEGER freq{};

		if (sysUsage)
		{
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
		}
		//auto t1 = std::chrono::high_resolution_clock::now();
#endif

		my_sys.second->FixedUpdate(dt);

#ifdef _EDITOR

		if (sysUsage)
		{
			QueryPerformanceCounter(&end);
			elapsed.QuadPart = end.QuadPart - start.QuadPart;
			elapsed.QuadPart *= 1000000;
			elapsed.QuadPart /= freq.QuadPart;

			if (sysUsage->size() > i)
				sysUsage->at(i) += (float)elapsed.QuadPart;
			else
				sysUsage->push_back((float)elapsed.QuadPart);
			++i;
		}
		//auto t2 = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		//SYS_EDITOR->GetSystemsUsage()->push_back(duration);
#endif
	}
}

void MySystemManager::Update()
{
#ifdef _EDITOR
	int i = 0;
#endif

	for (auto my_sys : Systems)
	{
#ifdef _EDITOR
		std::vector<float>* sysUsage = SYS_EDITOR->GetSystemsUsage();

		LARGE_INTEGER start{}, end{}, elapsed{};
		LARGE_INTEGER freq{};

		if (sysUsage)
		{
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
		}
		//auto t1 = std::chrono::high_resolution_clock::now();
#endif
		my_sys.second->Update();

#ifdef _EDITOR

		if (sysUsage)
		{
			QueryPerformanceCounter(&end);
			elapsed.QuadPart = end.QuadPart - start.QuadPart;
			elapsed.QuadPart *= 1000000;
			elapsed.QuadPart /= freq.QuadPart;

			if (sysUsage->size() > i)
				sysUsage->at(i) += (float)elapsed.QuadPart;
			else
				sysUsage->push_back((float)elapsed.QuadPart);
			++i;
		}
		//auto t2 = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		//SYS_EDITOR->GetSystemsUsage()->push_back(duration);
#endif
	}
}

//== Game Asserts clear

inline ENGINE_API void MySystemManager::GameExit() {
#ifndef _EDITOR
	MessageTogglePlay isPlaying(false, "TogglePlay");
	MessageTogglePlay isPlaying_1(false, "BeforePlay");
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying_1);
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying);
#endif

	for (auto my_sys : Systems)
		if (my_sys.first != S_PRIORITY::SP_SCENEMANAGER)
			my_sys.second->GameExit();
	Systems[S_PRIORITY::SP_SCENEMANAGER]->GameExit();
}
