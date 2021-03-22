#pragma once
#include "LevelEditor.h"
#include "..\..\FMOD\inc\fmod.hpp"

class LevelEditorAudio : public LE_WinBase_Derived<LevelEditorAudio>
{
public :
	LevelEditorAudio() {}
	~LevelEditorAudio() {}

	void Start() override {}
	void Init() override;
	void Run() override;
	void GameExit() override {}
	void Exit() override {}
};

