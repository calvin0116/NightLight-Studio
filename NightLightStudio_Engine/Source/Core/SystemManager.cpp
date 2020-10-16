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
	Systems[S_PRIORITY::SP_SCENEMANAGER] = SYS_SCENE_MANAGER;
	Systems[S_PRIORITY::SP_PHYSICS] = NS_PHYSICS::SYS_PHYSICS;
	Systems[S_PRIORITY::SP_COMPONENT] = SYS_COMPONENT;
	
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
