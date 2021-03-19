#pragma once
#include "LevelEditor.h"


class LevelEditorAudio : public LE_WinBase_Derived<LevelEditorAudio>
{
private:
	std::vector<std::string> Audios; // index, path
public :
	LevelEditorAudio() {}
	~LevelEditorAudio() {}

	void Start() override {}
	void Init() override;
	void Run() override;
	void GameExit() override {}
	void Exit() override {}
};

