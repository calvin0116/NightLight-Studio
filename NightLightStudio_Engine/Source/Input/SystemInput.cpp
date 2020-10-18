#pragma once
#include "SystemInput.h"
#include "../Window/WndSystem.h"
#include <iostream>

using namespace NS_INPUT;

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
}

void SystemInput::Free()
{
}

void SystemInput::Exit()
{
	DestroyInstance();
}

NS_INPUT::SystemKeyPress& SystemInput::GetSystemKeyPress()
{
	return _siKeyPress;
}

NS_INPUT::SystemController& SystemInput::GetSystemController()
{
	return _siCtrler;
}

NS_INPUT::SystemMousePosition& SystemInput::GetSystemMousePos()
{
	return _siMousePos;
}
