/***********************************************************************************
Brief Description :
	System Interface class for function needed to be called by system
************************************************************************************/
#pragma once
#include "../../framework.h"

#include "..//Component/ComponentManager.h"
#include "MySystem.h"
#include "Singleton.h"
#include "..//Graphics/SystemGraphics.h"

class ENGINE_API MySystemManager : public ISystem, public Singleton<MySystemManager>
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
	void OnFirstStart();

	//== Asserts importing / additional memory allocation phase
	void EarlyLoad() { for (auto my_sys : Systems) my_sys.second->EarlyLoad(); };
	void Load() { for (auto my_sys : Systems) my_sys.second->Load(); };
	void LateLoad() { for (auto my_sys : Systems) my_sys.second->LateLoad(); };

	//== Game / Usage of assert phase
	void EarlyInit() { for (auto my_sys : Systems) my_sys.second->EarlyInit(); };
	void Init() { for (auto my_sys : Systems) my_sys.second->Init(); };
	void LateInit() { for (auto my_sys : Systems) my_sys.second->LateInit(); }

	//== Game / System running loop
	bool FixedUpdate() { 
		for (auto my_sys : Systems) 
			if(!my_sys.second->FixedUpdate())
				return false;
		return true;
	};
	bool Update() { 
		for (auto my_sys : Systems)
			if (!my_sys.second->Update())
				return false;
		return true;
	};
	bool LateUpdate() { 
		for (auto my_sys : Systems)
			if (!my_sys.second->LateUpdate())
				return false;
		return true;
	};

	//== Game info clear
	void Exit() { for (auto my_sys : Systems) my_sys.second->Exit(); };
	//== Asserts clear
	void Unload() { for (auto my_sys : Systems) my_sys.second->Unload(); };
	//Memory deallocation phase
	void Free() {}

	//== Helper function

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

	bool CombineUpdate()
	{
		if(!FixedUpdate()) return false;
		if(!Update()) return false;
		if(!LateUpdate()) return false;

		return true;
	}

	//Getting system
	MySystem* operator[] (S_PRIORITY sys_p)
	{
		return Systems[sys_p];
	}
};

//Access point for all system
static  MySystemManager* SYS_MAN = MySystemManager::GetInstance();