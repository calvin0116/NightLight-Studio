#pragma once
#include "../../framework.h"
#include "CoreGraphics.h"
#include "../Core/MySystem.h"
#include "../IO/Json/Config.h"

//~~! Sample System
// ENGINE_API / __declspec(dllexport) < is for virtual function being overriding from Interface class
// to work for dll
//!!!Not adding ENGINE_API/__declspec(dllexport) will result in linker error
class ENGINE_API SystemGraphics : public MySystem, public Singleton<SystemGraphics>
{
private:
	friend Singleton<SystemGraphics>;
	CoreGraphics _coreGraphics;

protected:
	SystemGraphics() {};
	~SystemGraphics() {};
public:

	 void OnFirstStart();
	 void Load();
	 void Init();
	 bool Update() ;//float dt);
	 void Exit() ;
	 void Free() ;
	 void Unload() ;
	
};

//GLOBAL pointer to an instance of graphic system
//== Good to have for every system so that you dont need to always get instance
static SystemGraphics* SYS_GRAPHIC = SystemGraphics::GetInstance();