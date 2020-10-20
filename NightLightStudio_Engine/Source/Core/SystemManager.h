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
	//== Memory allocation phase / Load all system
	ENGINE_API void StartUp(HINSTANCE&);
	ENGINE_API void Init() { for (auto my_sys : Systems) my_sys.second->Init(); };


	//== Asserts importing
	ENGINE_API void GameLoad() { for (auto my_sys : Systems) my_sys.second->GameLoad(); };
	//== Game / Usage of assert phase
	//ENGINE_API void EarlyInit() { for (auto my_sys : Systems) my_sys.second->EarlyInit(); };
	ENGINE_API void GameInit() { for (auto my_sys : Systems) my_sys.second->GameInit(); };
	//ENGINE_API void LateInit() { for (auto my_sys : Systems) my_sys.second->LateInit(); }

	//== Game / System running loop
	ENGINE_API void FixedUpdate() {	for (auto my_sys : Systems) my_sys.second->FixedUpdate();};
	ENGINE_API void Update() {	for (auto my_sys : Systems) my_sys.second->Update();	};


	//== Game Asserts clear
	ENGINE_API void GameExit() { for (auto my_sys : Systems) my_sys.second->GameExit(); };

	//Memory deallocation phase without dependancy
	ENGINE_API void Free() {
		for (auto my_sys : Systems) my_sys.second->Free(); };
	//Memory deallocation phase without dependancy
	ENGINE_API void Exit() { 
		for (auto my_sys : Systems) my_sys.second->Exit();
		DestroyInstance();
	};

	//== Helper function
	//Getting system
	ENGINE_API MySystem* operator[] (S_PRIORITY sys_p)
	{
		return Systems[sys_p];
	}
};

//Access point for all system
static  MySystemManager* SYS_MAN = MySystemManager::GetInstance();