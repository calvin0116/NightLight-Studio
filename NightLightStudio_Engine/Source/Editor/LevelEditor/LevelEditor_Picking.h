#ifndef LEVEL_EDITOR_PICKING_HELPER_CLASS
#define LEVEL_EDITOR_PICKING_HELPER_CLASS

#include <vector>
#include <algorithm>

#include "../../Component/Components.h"

class LE_Picking
{
	std::vector<int> _hitEntities;
	int _hitEntityNum = 0;
public:
	LE_Picking();
	~LE_Picking();

	void LE_PickingRun();
};

#endif
