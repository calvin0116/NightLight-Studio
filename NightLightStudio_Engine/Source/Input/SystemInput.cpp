#pragma once
#include "SystemInput.h"
#include <iostream>

using namespace SystemInput_ns;

SystemInput::SystemInput()
	: _siKeyPress(), _siCtrler(-1, false), _siMousePos()
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
	_siKeyPress.ALL_THE_KEYS();
}

bool SystemInput::Update()
{
	//std::cout << "System::Input::Update::BIG" << std::endl;
	_siKeyPress.Update();
	_siCtrler.Update();
	_siMousePos.Update();

	if (_siKeyPress.GetKeyHold(IKEY_A))
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

SystemInput_ns::SystemKeyPress& SystemInput::GetSystemKeyPress()
{
	return _siKeyPress;
}

SystemInput_ns::SystemController& SystemInput::GetSystemController()
{
	return _siCtrler;
}

SystemInput_ns::SystemMousePosition& SystemInput::GetSystemMousePos()
{
	return _siMousePos;
}
