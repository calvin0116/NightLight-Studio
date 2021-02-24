#pragma once
#include "SystemInput.h"
#include "../Window/WndSystem.h"
#include <iostream>

// Tracy
#include "../tracy-master/Tracy.hpp"

using namespace SystemInput_ns;

SystemInput::SystemInput()
	: _siKeyPress(), _siCtrler(-1, false), _siMousePos()
{
}

void SystemInput::Load()
{
}

void SystemInput::Init()
{
	std::cout << "System::Input::Init" << std::endl;
	_siKeyPress.SetWindow(NS_WINDOW::SYS_WINDOW->GetHandlerToWindow());
	_siCtrler.SetWindow(NS_WINDOW::SYS_WINDOW->GetHandlerToWindow());
	_siMousePos.SetWindow(NS_WINDOW::SYS_WINDOW->GetHandlerToWindow());
}

void SystemInput::Update()
{
	//std::cout << "System::Input::Update::BIG" << std::endl;
	_siKeyPress.Update();
	_siCtrler.Update();
	_siMousePos.Update();

	// Tracy
	ZoneScoped
}

void SystemInput::Free()
{
}

void SystemInput::Exit()
{
	DestroyInstance();
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
