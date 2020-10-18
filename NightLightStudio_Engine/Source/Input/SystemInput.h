#ifndef SI_SYSTEM_INPUT
#define SI_SYSTEM_INPUT

#include "SI_KeyPress.h"
#include "SI_MousePos.h"
#include "SI_Controller.h"
#include "../../framework.h"
#include "../Core/MySystem.h"

class SystemInput : public MySystem, public Singleton<SystemInput>
{
private:
	friend Singleton<SystemInput>;
	NS_INPUT::SystemKeyPress _siKeyPress;
	NS_INPUT::SystemController _siCtrler;
	NS_INPUT::SystemMousePosition _siMousePos;

protected:
	ENGINE_API SystemInput();
	ENGINE_API ~SystemInput() = default;

public:
	//ENGINE_API void OnFirstStart() override;

	ENGINE_API void Load() override;

	ENGINE_API void Init() override;

	ENGINE_API void Update() override;

	ENGINE_API void Free() override;

	ENGINE_API void Exit() override;


	// Access individual System Parts to use their functions or something I guess
	ENGINE_API NS_INPUT::SystemKeyPress& GetSystemKeyPress();

	ENGINE_API NS_INPUT::SystemController& GetSystemController();

	ENGINE_API NS_INPUT::SystemMousePosition& GetSystemMousePos();
};

//GLOBAL pointer to an instance of graphic system
//== Good to have for every system so that you dont need to always get instance
static SystemInput* SYS_INPUT = SystemInput::GetInstance();

#endif
