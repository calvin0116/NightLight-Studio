#ifndef LEVELEDITOR_CONSOLE
#define LEVELEDITOR_CONSOLE

#include "LevelEditor.h"
#include <map>

class ConsoleLog : public LE_WinBase_Derived<ConsoleLog>
{
	const unsigned MAX_SIZE_INPUTS = 1000;
	const unsigned MAX_SIZE_HISTORY = 100;
	std::string _inputBuffer;
	std::vector<std::string> _inputItems;
	std::vector<std::string> _inputHistory;
	int _historyPos;
	std::map<const std::string, std::function<void()>> _commands;

public:
	ConsoleLog() : _inputBuffer{}, _inputItems{}, _inputHistory{}, _historyPos{ -1 }, _commands{}
	{}
	~ConsoleLog() {}

	void Init() override;

	void Run() override;

	void Exit() override {}

	void AddLog(const std::string& item);

	void AddCommand(const std::string& command, std::function<void()> fn);
	void ExecCommand(const std::string& command);

	void ClearLog();

	int TextEditCallback(ImGuiInputTextCallbackData* data);
};

#endif
