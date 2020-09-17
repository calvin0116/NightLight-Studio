#pragma once


class ISystem
{
public:
	virtual void Load();

	virtual void Init();

	virtual void Update();

	virtual void Exit();

	virtual void Free();

	virtual void Unload();

};