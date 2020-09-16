/***********************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : main.h
Author : Ngian Teck Wei (100%)
DP email : teckwei.ngian@digipen.edu
Course : GAM200F19-A
Brief Description :

************************************************************************************/
#pragma once
//#include "Singleton.h"

//template <typename T>
struct ISystem 
{
public:
	ISystem() {};
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
	virtual void FixedUpdate() {};
	virtual void Update() {} ;
	virtual void LateUpdate() {};

	virtual void Exit() {};
	virtual void Unload() {};

	virtual ~ISystem() {};
};