#pragma once
#include <string>
#include "../../framework.h"
#include "../Component/ComponentManager.h"
/*
class PrefabManager : public Singleton<PrefabManager>
{



};
*/
namespace Prefab_Function {
	ENGINE_API int PrefabReadAndCreate(std::string file);

	ENGINE_API void WritePrefab(std::string file, Entity& prefab_ent);
}

namespace PFunc = Prefab_Function;