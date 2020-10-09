/***********************************************************************************
Brief Description :
	System Interface class for function needed to be called by system
************************************************************************************/
#pragma once
#include "../../framework.h"

#include "..//Component/ComponentManager.h"
//System includes
#include "MySystem.h"
#include "Singleton.h"

//System to manage
#include "../IO/SystemIO.h"



class  MySystemManager : public Singleton<MySystemManager>
{
private:
	//For singleton to access constructor / destructor
	friend Singleton<MySystemManager>;

	//Map of system according to their piority
	std::map<S_PRIORITY, MySystem*> Systems;
	//std::vector< MySystem *
public:
	//List of function calling for all system
	//== Memory allocation phase
	ENGINE_API void InitSystem(HINSTANCE&);

	//== Asserts importing / additional memory allocation phase
	//ENGINE_API void EarlyLoad() { for (auto my_sys : Systems) my_sys.second->EarlyLoad(); };
	ENGINE_API void LoadGame() { for (auto my_sys : Systems) my_sys.second->Load(); };
	//ENGINE_API void LateLoad() { for (auto my_sys : Systems) my_sys.second->LateLoad(); };

	//== Game / Usage of assert phase
	//ENGINE_API void EarlyInit() { for (auto my_sys : Systems) my_sys.second->EarlyInit(); };
	ENGINE_API void OnGameStart() { for (auto my_sys : Systems) my_sys.second->Init(); };
	//ENGINE_API void LateInit() { for (auto my_sys : Systems) my_sys.second->LateInit(); }

	//== Game / System running loop
	ENGINE_API bool FixedUpdate() {
		for (auto my_sys : Systems) 
			if(!my_sys.second->FixedUpdate())
				return false;
		return true;
	};
	ENGINE_API bool Update() {
		for (auto my_sys : Systems)
			if (!my_sys.second->Update())
				return false;
		return true;
	};
	ENGINE_API bool LateUpdate() {
		for (auto my_sys : Systems)
			if (!my_sys.second->LateUpdate())
				return false;
		return true;
	};

	//== Game info clear
	ENGINE_API void Exit() { for (auto my_sys : Systems) my_sys.second->Exit(); };
	//== Asserts clear
	ENGINE_API void Unload() { for (auto my_sys : Systems) my_sys.second->Unload(); };
	//Memory deallocation phase
	ENGINE_API void Free() { for (auto my_sys : Systems) my_sys.second->Free(); };

	//== Helper function

	// All this function for now is just placed here for normal use
	// Sequencing will be improved later on
	ENGINE_API void CombineLoad()
	{
		EarlyLoad();
		Load();
		LateLoad();
	}

	ENGINE_API void CombineInit()
	{
		EarlyInit();
		Init();
		LateInit();
	}

	ENGINE_API bool CombineUpdate()
	{
		if(!FixedUpdate()) return false;
		if(!Update()) return false;
		if(!LateUpdate()) return false;

		return true;
	}

	//Getting system
	ENGINE_API MySystem* operator[] (S_PRIORITY sys_p)
	{
		return Systems[sys_p];
	}
};

//Access point for all system
static  MySystemManager* SYS_MAN = MySystemManager::GetInstance();