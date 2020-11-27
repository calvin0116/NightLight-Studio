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

void SystemEditor::LE_ConsoleOut(const std::string& out)
{
	_editor.LE_ConsoleOut(out);
}

#endif

void ED_OUT(const std::string& out)
{
#ifdef _EDITOR
	SYS_EDITOR->LE_ConsoleOut(out);
#endif
}

