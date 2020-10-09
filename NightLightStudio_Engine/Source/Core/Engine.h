#pragma once
//Noted from Teck Wei: I shifted your ENGINE_API to framework.h header as i need to use it too
#include "../../framework.h"
#include "SystemManager.h"
#include <windows.h>




class ENGINE_API FluffyUnicornEngine
{
	enum ENGINESTATE
	{
		ENGINE_QUIT = 0,
		ENGINE_UPDATE,
	};
	ENGINESTATE _engineState;
	float _prevdt;
	HINSTANCE hinstance;
public:
	FluffyUnicornEngine() = default;
	~FluffyUnicornEngine() = default;

	void Init(HINSTANCE&);
	void Run();
	//**! Have and Exit or smth
	void Exit();
	
};


//class FluffyUnicornEngine
//{
//	enum ENGINESTATE
//	{
//		ENGINE_QUIT = 0,
//		ENGINE_UPDATE,
//	};
//	ENGINESTATE _engineState;
//	float _prevdt;
//public:
//	FluffyUnicornEngine() = default;
//	~FluffyUnicornEngine() = default;
//
//	void Init();
//	void Run();
//};
//