#pragma once
#include "SystemInput.h"
#include <iostream>

using namespace SystemInput_ns;

SystemInput::SystemInput()
	: _SIKeyPress(), _SICtrler(-1, false), _SIMousePos()
{
}

void SystemInput::OnFirstStart()
{
}

void SystemInput::Load()
{
}

void SystemInput::Init()
{
	_SIKeyPress.ALL_THE_KEYS();
}

bool SystemInput::Update()
{
	// Testing only
	//std::cout << "System::Input::Update::BIG" << std::endl;
	_SIKeyPress.Update();
	_SICtrler.Update();
	_SIMousePos.Update();

	if (_SIKeyPress.GetKeyHold(IKEY_A))
	{
		std::cout << "System::Input::Update::PRESSING A" << std::endl;
	}

	return true;
}

void SystemInput::Exit()
{
}

void SystemInput::Unload()
{
}

ENGINE_API SystemInput_ns::SystemKeyPress& SystemInput::GetSystemKeyPress()
{
	return _SIKeyPress;
}

ENGINE_API SystemInput_ns::SystemController& SystemInput::GetSystemController()
{
	return _SICtrler;
}

ENGINE_API SystemInput_ns::SystemMousePosition& SystemInput::GetSystemMousePos()
{
	return _SIMousePos;
}
