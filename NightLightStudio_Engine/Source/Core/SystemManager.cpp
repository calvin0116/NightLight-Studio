#include "SystemManager.h"
#include "Systems.h"


#include "../IO/Json/Config.h"

#include <functional>

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

	//SYS_COMPONENT->Clear(NS_COMPONENT::COMPONENT_MAIN);
	//SYS_COMPONENT->Clear();

	//Entity newEntity = G_ECMANAGER->BuildEntity();
	//ComponentCollider* OBBT = newEntity.AddComponent<ComponentCollider>();
	//ComponentCollider* OBBT = G_ECMANAGER->AddComponent<ComponentCollider>(newEntity);
	//ComponentCollider OBB1(COLLIDERS::OBB);
	//*OBBT = OBB1;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Systems OnFirst start
	for (auto my_sys : Systems)
		my_sys.second->Load();
	// Systems OnFirst start END
	///////////////////////////////////////////////////////////////////////////////////////////////



}

void MySystemManager::Update()
{
	for (auto my_sys : Systems)
	{
#ifdef _EDITOR
		LARGE_INTEGER start, end, elapsed;
		LARGE_INTEGER freq;

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);
		//auto t1 = std::chrono::high_resolution_clock::now();
#endif
		my_sys.second->Update();

#ifdef _EDITOR
		QueryPerformanceCounter(&end);
		elapsed.QuadPart = end.QuadPart - start.QuadPart;
		elapsed.QuadPart *= 1000000;
		elapsed.QuadPart /= freq.QuadPart;
		SYS_EDITOR->GetSystemsUsage()->push_back((float)elapsed.QuadPart);
		//auto t2 = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		//SYS_EDITOR->GetSystemsUsage()->push_back(duration);
#endif
	}
}
