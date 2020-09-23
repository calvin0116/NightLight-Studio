#pragma once

class SystemEditor
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