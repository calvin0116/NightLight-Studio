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
	//Load with different timing to suit different system
	virtual void EarlyLoad() {};
	virtual void Load() {};
	virtual void LateLoad() {};
	
	//Init with different timing to suit different system
	virtual void EarlyInit() {};
	virtual void Init() {};
	virtual void LateInit() {};
	
	virtual void FixedUpdate() {};
	virtual void Update() {} ;
	virtual void LateUpdate() {};

	virtual void Exit() {};
	virtual void Unload() {};

	virtual ~ISystem() {};
};