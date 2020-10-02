#ifndef SI_SYSTEM_INPUT
#define SI_SYSTEM_INPUT

#include "SI_KeyPress.h"
#include "SI_MousePos.h"
#include "SI_Controller.h"
#include "../../framework.h"
#include "../Core/MySystem.h"


#endif

class SystemInput : public MySystem, public Singleton<SystemInput>
{
private:
	friend Singleton<SystemInput>;
	SystemInput_ns::SystemKeyPress _siKeyPress;
	SystemInput_ns::SystemController _siCtrler;
	SystemInput_ns::SystemMousePosition _siMousePos;

protected:
	ENGINE_API SystemInput();
	ENGINE_API ~SystemInput() = default;

public:
	ENGINE_API void OnFirstStart() override;

	ENGINE_API void Load() override;

	ENGINE_API void Init() override;

	ENGINE_API bool Update() override;

	ENGINE_API void Exit() override;

	ENGINE_API void Unload() override;

	// Access individual System Parts to use their functions or something I guess
	ENGINE_API SystemInput_ns::SystemKeyPress& GetSystemKeyPress();

	ENGINE_API SystemInput_ns::SystemController& GetSystemController();

	ENGINE_API SystemInput_ns::SystemMousePosition& GetSystemMousePos();
};

//GLOBAL pointer to an instance of graphic system
//== Good to have for every system so that you dont need to always get instance
static SystemInput* SYS_INPUT = SystemInput::GetInstance();
