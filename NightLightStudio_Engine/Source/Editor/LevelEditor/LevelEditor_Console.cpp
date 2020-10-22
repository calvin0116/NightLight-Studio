#include "LevelEditor_Console.h"

void ConsoleLog::Init()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize(ImVec2(320, 320), ImGuiCond_FirstUseEver);
}

void ConsoleLog::Run()
{
	//for (unsigned i = 0; i < _inputItems.size(); ++i)
	//	_levelEditor->LE_AddText(_inputItems[i].c_str());

	_levelEditor->LE_AddButton("Debug Log", [&]() { AddLog("Test Debug Log"); });
	ImGui::SameLine();
	_levelEditor->LE_AddButton("Clear Log", [&]() { ClearLog(); });

	_levelEditor->LE_AddChildWindow("ConsoleLog", ImVec2(0, -(ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing())),
		{
			[&]()
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

				for (int i = 0; i < _inputItems.size(); i++)
				{
					const char* item = _inputItems[i].c_str();

					// Normally you would store more information in your item than just a string.
					// (e.g. make Items[] an array of structure, store color/type etc.)
					ImVec4 color;
					bool has_color = false;
					if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
					else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
					if (has_color)
						ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextUnformatted(item);
					if (has_color)
						ImGui::PopStyleColor();
				}

				ImGui::PopStyleVar();
			}
		}, true, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Separator();
	_levelEditor->LE_AddText("Input");
	ImGui::SameLine();
	ImGuiInputTextFlags inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	_levelEditor->LE_AddInputText("##Input", _inputBuffer, 100, inputFlag,
		[&]() 
		{
			if (_inputBuffer != "")
			{
				AddLog(_inputBuffer);
				if (_inputHistory.size() >= MAX_SIZE_HISTORY)
					_inputHistory.erase(std::begin(_inputItems));
				_inputHistory.push_back(_inputBuffer);
				_inputBuffer.clear();
				_historyPos = -1;
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SetKeyboardFocusHere(-1);
		},
		[](ImGuiInputTextCallbackData* data)
		{
			ConsoleLog* console = static_cast<ConsoleLog*>(data->UserData);
			return console->TextEditCallback(data);
		},
		(void*)this);

}

void ConsoleLog::AddLog(const std::string& item)
{
	if (_inputItems.size() >= MAX_SIZE_INPUTS)
		_inputItems.erase(std::begin(_inputItems));
	_inputItems.push_back(item);
	ExecCommand(item);
}

void ConsoleLog::AddCommand(const std::string& command, std::function<void()> fn)
{
	_commands.emplace(command, fn);
}

void ConsoleLog::ExecCommand(const std::string& command)
{
	if (_commands.find(command) != std::end(_commands))
		_commands[command]();
}

void ConsoleLog::ClearLog()
{
	_inputItems.clear();
	_inputHistory.clear();
}

int ConsoleLog::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
		// Tabbing
	case ImGuiInputTextFlags_CallbackCompletion:
	{
		break;
	}
		// Key up or down
	case ImGuiInputTextFlags_CallbackHistory:
	{
		const int prev_history_pos = _historyPos;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (_historyPos == -1)
				_historyPos = (int)_inputHistory.size() - 1;
			else if (_historyPos > 0)
				_historyPos--;
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (_historyPos != -1)
				if (++_historyPos >= _inputHistory.size())
					_historyPos = -1;
		}

		// A better implementation would preserve the data on the current input line along with cursor position.
		if (prev_history_pos != _historyPos)
		{
			std::string history_str = (_historyPos >= 0) ? _inputHistory[_historyPos] : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str.c_str());
		}

		break;
	}
	}
	return 0;
}
