#pragma once
#include "Header.h"
#include "MySystem.h"
#include "Singleton.h"

//Managers
#include "MyWindowManager.h"
#include "PhysicManager.h"

class MySystemManager : public ISystem, public Singleton<MySystemManager>
{
private:
	//For singleton to access constructor / destructor
	friend Singleton<MyWindowManager>;

	//Map of system according to their piority
	std::map<SYS_PIORIOTY, MySystem*> Systems;
	//std::vector< MySystem *
public:
	void StartUp()
	{
		Systems[SYS_PHYSICS] = PhysicManager::GetInstance();
		// === Insert your system here to get them running === //
		// === Please follow how PhysicManager is created  === // 
	}

	//List of function calling for all system

	void EarlyLoad() { for (auto my_sys : Systems) my_sys.second->EarlyLoad(); };
	void Load() { for (auto my_sys : Systems) my_sys.second->Load(); };
	void LateLoad() { for (auto my_sys : Systems) my_sys.second->LateLoad(); };


	void EarlyInit() { for (auto my_sys : Systems) my_sys.second->EarlyInit(); };
	void Init() { for (auto my_sys : Systems) my_sys.second->Init(); };
	void LateInit() { for (auto my_sys : Systems) my_sys.second->LateInit(); }

	void FixedUpdate() { for (auto my_sys : Systems) my_sys.second->FixedUpdate(); };
	void Update() { for (auto my_sys : Systems) my_sys.second->Update(); };
	void LateUpdate() { for (auto my_sys : Systems) my_sys.second->LateUpdate(); };

	void Exit() { for (auto my_sys : Systems) my_sys.second->Exit(); };
	void Unload() { for (auto my_sys : Systems) my_sys.second->Unload(); };
	void DestroyInstance() {}

	//Helper function

	// All this function for now is just placed here for normal use
	// Sequencing will be improved later on
	void CombineLoad()
	{
		EarlyLoad();
		Load();
		LateLoad();
	}

	void CombineInit()
	{
		EarlyInit();
		Init();
		LateInit();
	}

	void CombineUpdate()
	{
		FixedUpdate();
		Update();
		LateUpdate();
	}

	//Getting system
	MySystem * operator[] (SYS_PIORIOTY sys_p)
	{
		return Systems[sys_p];
	}
};

//Access point for all system
static  MySystemManager* const SYS_MAN = MySystemManager::GetInstance();