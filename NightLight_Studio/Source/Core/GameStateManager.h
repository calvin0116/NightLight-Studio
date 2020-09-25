#pragma once

#include "..//Component/ComponentManager.h"

class GameStateManager
{
public:

	enum GAMESTATE
	{
		GAMESTATE_UPDATE,
		GAMESTATE_UPDATE_STEP,
		GAMESTATE_PAUSE,
		GAMESTATE_PAUSE_STEP,
		GAMESTATE_PAUSE_OPTIONS,

		GAMESTATE_LOAD,
		GAMESTATE_INIT,
		GAMESTATE_FREE,
		GAMESTATE_UNLOAD,

		GAMESTATE_QUIT,

		GAMESTATE_END
	};

private:
	GAMESTATE _gamestate;
	GAMESTATE _gamestateNext;

	ComponentManager _mgrCom;
	//ComponentManager::ComponentSetManager _csmgr0;

	// scene
	int _scnInd; // index of the current scene
	int _scnIndNext; // index of the next scene

public:
	GameStateManager();

	~GameStateManager();

	void OnFirstStart();

	void Load();

	void Init();

	int Update(float dt, int step);

	void Free();

	void Unload();

	int getScn();

	void setScn(int scn);

};

extern GameStateManager G_GSM;