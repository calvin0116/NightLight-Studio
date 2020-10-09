#include "SystemEditor.h"
#include "../Window/WndSystem.h"

#ifdef _EDITOR

SystemEditor::SystemEditor()
	: _editor {}
{
}

void SystemEditor::OnFirstStart()
{
}

void SystemEditor::Load()
{
}

void SystemEditor::Init()
{
	_editor.Init(NS_WINDOW::SYS_WINDOW->GetHandlerToWindow());
}

bool SystemEditor::Update()
{
	_editor.Update();
	return true;
}

void SystemEditor::Exit()
{
	_editor.Exit();
}

void SystemEditor::Free()
{
}

void SystemEditor::Unload()
{
}

#endif

