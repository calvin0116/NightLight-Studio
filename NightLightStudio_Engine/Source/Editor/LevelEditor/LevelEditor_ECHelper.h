#pragma once
#include "../../../framework.h"
#include "../../Core/Singleton.h"

class LE_ECHelper: public Singleton<LE_ECHelper>
{
protected:
	friend Singleton<LE_ECHelper>;

	LE_ECHelper() {};
	~LE_ECHelper() {};
public:

	//First = entity id, second = selected bool
	std::map<int, bool> selected_ents;
	//First = level editor id, second = entity id
	std::map<int, int> le_id_to_object_id;
};
#ifdef _EDITOR

static LE_ECHelper* LE_ECHELPER = LE_ECHelper::GetInstance();

#endif //  
