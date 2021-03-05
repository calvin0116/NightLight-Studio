#ifndef LEVELEDITOR_CONSOLE
#define LEVELEDITOR_CONSOLE

#include "LevelEditor.h"
#include <map>
#include <any>

// Stores a DO function, UNDO function, and a single value that stores values that you want to use
typedef std::function<std::any(std::any)> COMMAND;
//typedef std::function<std::any(std::any,bool)> COMMAND_2;
typedef std::pair<COMMAND, COMMAND> DO_UNDO_COMMAND;

class ConsoleLog : public LE_WinBase_Derived<ConsoleLog>
{
	const unsigned MAX_SIZE_INPUTS = 1000;
	const unsigned MAX_SIZE_HISTORY = 100;
	std::string _inputBuffer;
	std::vector<std::string> _inputItems;
	std::vector<std::string> _inputHistory;
	int _historyPos;

	std::vector<std::pair<std::string, std::any>> _inputCommands;
	std::vector<std::pair<std::string, std::any>> _inputCommandsRedo;
	std::map<const std::string, DO_UNDO_COMMAND> _commands;

	bool _scrollToBottom;

	const size_t _maxCommands = 100;

	bool _saveOnExit;
	bool _saveOnLog;

	// TO REMOVE
	int _somevalue = 0;

	void SaveToFile(std::string filePath);

public:
	ConsoleLog() : _inputBuffer{}, _inputItems{}, _inputHistory{}, _historyPos{ -1 }, _commands{}, _inputCommands{}, _scrollToBottom{ false }
		, _saveOnExit{ true }, _saveOnLog{ true }
	{}
	~ConsoleLog() {}

	void Start() override;

	void Init() override;

	void Run() override;

	void Exit() override {}

	void End() override;

	void AddLog(const std::string item);

	// COMMAND -> std::pair < std::function <void(std::any)>, std::any >
	// std::function <std::any(std::any)> -> The function that takes in an arbitrary value state to run, and returns the initial value state.
	// Please note:
	// The return value of the Do function will be the argument used for the Undo function
	// The return value of the Undo function will be the argument used for the Do function
	void AddCommand(const std::string command, COMMAND doFN, COMMAND undoFN = nullptr);

	void RunCommand(const std::string command, std::any value = nullptr);

	void UndoLastCommand();
	void RedoLastCommand();

	void ClearLog();

	int TextEditCallback(ImGuiInputTextCallbackData* data);
};

#endif
