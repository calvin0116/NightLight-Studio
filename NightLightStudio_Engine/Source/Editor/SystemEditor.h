#ifndef SI_SYSTEM_EDITOR
#define SI_SYSTEM_EDITOR

#include "../../framework.h"
#include "../Core/MySystem.h"
#include "LevelEditor/LevelEditor.h"

#ifdef _EDITOR

class SystemEditor : public MySystem, public Singleton<SystemEditor>
{
private:
	friend Singleton<SystemEditor>;

	LevelEditor _editor;

protected:
	ENGINE_API SystemEditor();
	ENGINE_API ~SystemEditor() = default;

public:
	ENGINE_API void Load() override;

	ENGINE_API void Init() override;

	ENGINE_API void Update() override;

	ENGINE_API void Exit() override;

	ENGINE_API void Free() override;

	ENGINE_API std::vector<float>* GetSystemsUsage();

	ENGINE_API void LE_ConsoleOut(const std::string& out);
};

//GLOBAL pointer to an instance of graphic system
//== Good to have for every system so that you dont need to always get instance
static SystemEditor* SYS_EDITOR = SystemEditor::GetInstance();

void ED_OUT(const std::string& out);

#endif

#endif