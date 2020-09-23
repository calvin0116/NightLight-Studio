#pragma once
#include "../../framework.h"

class ENGINE_API ISystem
{
public:
	ISystem() {};
	virtual ~ISystem() {};

	//First start up function
	virtual void OnFirstStart() {};

	//*Load with different timing to suit different system*//
	// For basic uses, Just overwrite Load() function
	//
	// Early/Late load is used only when you need a certain system to do a certain action
	// before you can get what you need
	// E.g Graphics early load its window for all to use 
	//	   Level editor late load to get all the Component needed for Ui design
	virtual void EarlyLoad() {};
	virtual void Load() {};
	virtual void LateLoad() {};

	//*Init with different timing to suit different system*//
	//Same as load, Overwrite Init for basic uses
	//Others is see as you need
	virtual void EarlyInit() {};
	virtual void Init() {};
	virtual void LateInit() {};

	//*Update with different timing to suit different system*//
	//Same as load 
	//But there is one new function, FixedUpdate
	//FixedUpdate is used mostly Physics / events that needs to happen even when delta time is huge
	//Others is the same
	virtual bool FixedUpdate() { return true; };
	virtual bool Update() { return true; };
	virtual bool LateUpdate() { return true; };
	 
	//If need any early or late exit please tell me
	//For now i do not see any uses
	virtual void Exit() {};

	//Unload for normal resources, Late unload depended system (e.g. graphics)
	virtual void Unload() {};
	virtual void LateUnload() {};

};