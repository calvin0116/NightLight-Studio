#include "LevelEditor_Console.h"
#include "../../Input/SystemInput.h"

#include <iostream>

void ConsoleLog::Start()
{
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("EDITOR_UNDO_REDO", SystemInput_ns::IKEY_CTRL, "CTRL_Z_Y", SystemInput_ns::OnHold,
		[this]() 
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_Z))
				UndoLastCommand();
			if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_Y))
				RedoLastCommand();
		});

	
	AddCommand("TEST_COMMAND", 
		[this](std::any value)
		{
			// Initial state
			int ret = _somevalue;

			// THE ANY CAST MUST BE A SPECIFIC TYPE OR IT WILL FAIL
			// Undo takes in as a string
			std::string val = std::any_cast<std::string>(value);

			// Uses the value to set state
			_somevalue = std::stoi(val);

			// Debugging only
			AddLog("_someValue: " + std::to_string(_somevalue));

			// Returns previous state
			return ret;
		},
		[this](std::any value)
		{
			// Initial state (storing as a string for testing purposes)
			std::string ret = std::to_string(_somevalue);

			// THE ANY CAST MUST BE A SPECIFIC TYPE OR IT WILL FAIL
			// Do takes in an int
			int val = std::any_cast<int>(value);

			// Uses the value to set state
			_somevalue = val;

			// Debugging only
			AddLog("_someValue: " + std::to_string(_somevalue));

			// Returns previous state
			return ret;
		});

	// Running commands as per normal
	RunCommand("TEST_COMMAND", std::to_string(1));
	RunCommand("TEST_COMMAND", std::to_string(2));
	RunCommand("TEST_COMMAND", std::to_string(3));


	// Testing Undo Redo //
	// Values should be : 1,2,3, 2,1, 2, 1, 2,3, 2,1,0,
	UndoLastCommand();
	UndoLastCommand();
	RedoLastCommand();
	UndoLastCommand();
	RedoLastCommand();
	RedoLastCommand();
	RedoLastCommand();
	UndoLastCommand();
	UndoLastCommand();
	UndoLastCommand();
	UndoLastCommand();

	
}

void ConsoleLog::Init()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize(ImVec2(320, 320), ImGuiCond_FirstUseEver);
}

void ConsoleLog::Run()
{
	//for (unsigned i = 0; i < _inputItems.size(); ++i)
	//	_levelEditor->LE_AddText(_inputItems[i].c_str());

	_levelEditor->LE_AddButton("Debug Log", [this]() { AddLog("Test Debug Log"); });
	ImGui::SameLine();
	_levelEditor->LE_AddButton("Clear Log", [this]() { ClearLog(); });

	_levelEditor->LE_AddChildWindow("ConsoleLog", ImVec2(0, -(ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing())),
		[this]()
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

			if (_scrollToBottom) {
				ImGui::SetScrollHere(1.0f);
				_scrollToBottom = false;
			}
		}, true, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Separator();
	_levelEditor->LE_AddText("Input");
	ImGui::SameLine();
	ImGuiInputTextFlags inputFlag = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_NoUndoRedo;
	_levelEditor->LE_AddInputText("##Input", _inputBuffer, 100, inputFlag,
		[this]() 
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

// Might need to expand
void ConsoleLog::AddLog(const std::string& item)
{
	if (_inputItems.size() >= MAX_SIZE_INPUTS)
		_inputItems.erase(std::begin(_inputItems));
	_inputItems.push_back(item);

	// On new log, scroll to bottom
	_scrollToBottom = true;

	// RUN COMMAND HERE
	try
	{
		std::string command, value;

		std::size_t pos = item.find(" ");

		command = item.substr(0, pos);
		value = item.substr(pos + 1, item.size());

		RunCommand(command, value);
	}
	catch (const std::exception& e)
	{
		if (_inputItems.size() >= MAX_SIZE_INPUTS)
			_inputItems.erase(std::begin(_inputItems));
		std::string errorMsg = "[error] : ";
		errorMsg.append(e.what());
		_inputItems.push_back(errorMsg);
	}
}

void ConsoleLog::AddCommand(const std::string command, COMMAND doFN, COMMAND undoFN)
{
	_commands.emplace(command, DO_UNDO_COMMAND(doFN, undoFN));
}

void ConsoleLog::RunCommand(const std::string command, std::any value)
{
	decltype(_commands)::iterator iter = _commands.find(command);
	if (iter != std::end(_commands))
	{
		// Runs the function for DO
		COMMAND fn = iter->second.first;
		if (fn)
		{
			// Does not store if there is no undo functionality available
			if (iter->second.second)
			{
				// Stores the initial values of the command
				std::any returnValue = fn(value);
				_inputCommands.push_back(std::make_pair(command, returnValue));

				// SET MINIMUM NUMBER OF COMMANDS TO STORE
				if (_inputCommands.size() > _maxCommands)
					_inputCommands.erase(std::begin(_inputCommands));
			}
			else
				fn(value);

			// Clears all possible forward events to prevent issues
			_inputCommandsRedo.clear();
		}
	}
}

void ConsoleLog::UndoLastCommand()
{
	// Checks if there is anything left to undo
	if (_inputCommands.size())
	{
		decltype(_inputCommands)::value_type lastCommand = _inputCommands.back();
		std::string command = lastCommand.first;
		std::any value = lastCommand.second;

		// No need to check if it exists; if it's in vector, then it must be an existing command with undo functionality
		DO_UNDO_COMMAND iter = _commands[command];
		// Runs the function for UNDO
		COMMAND fn = iter.second;
		_inputCommandsRedo.push_back(std::make_pair(command, fn(value)));

		_inputCommands.pop_back();

	}
}

void ConsoleLog::RedoLastCommand()
{
	if (_inputCommandsRedo.size())
	{
		decltype(_inputCommandsRedo)::value_type lastCommand = _inputCommandsRedo.back();
		std::string command = lastCommand.first;
		std::any value = lastCommand.second;

		// No need to check if it exists; if it's in vector, then it must be an existing command with undo functionality
		DO_UNDO_COMMAND iter = _commands[command];
		// Runs the function for UNDO
		COMMAND fn = iter.first;
		_inputCommands.push_back(std::make_pair(command, fn(value)));

		_inputCommandsRedo.pop_back();
	}
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
