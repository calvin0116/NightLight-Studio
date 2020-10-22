#include "SystemEditor.h"
#include "../Window/WndSystem.h"

#ifdef _EDITOR

SystemEditor::SystemEditor()
	: _editor {}
{
}

void SystemEditor::Load()
{
}

void SystemEditor::Init()
{
	_editor.Init(NS_WINDOW::SYS_WINDOW->GetHandlerToWindow());
}

void SystemEditor::Update()
{
	_editor.Update();
}

void SystemEditor::Exit()
{
	_editor.Exit();
	DestroyInstance();
}

void SystemEditor::Free()
{
}

ENGINE_API std::vector<float>* SystemEditor::GetSystemsUsage()
{
	return _editor.LE_GetSystemsUsage();
}

#endif

