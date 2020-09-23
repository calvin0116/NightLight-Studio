#ifndef SI_SYSTEM_INPUT
#define SI_SYSTEM_INPUT

#include "SI_KeyPress.h"
#include "SI_MousePos.h"
#include "SI_Controller.h"

#endif

class SystemInput
{
public:
	void OnFirstStart();

	void Load();

	void Init();

	void Update(float dt);

	void Exit();

	void Free();

	void Unload();
};