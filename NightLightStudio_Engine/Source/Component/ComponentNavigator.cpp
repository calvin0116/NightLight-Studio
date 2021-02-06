#include "ComponentNavigator.h"
#include "../Component/ComponentManager.h"

inline void ComponentNavigator::FindClosestWP()
{
	std::pair<int, float> closest_wp = std::make_pair<int, float>(-1, std::numeric_limits<float>::max());
	TransformComponent* trans_comp = G_ECMANAGER->getEntity(this).getComponent<TransformComponent>();

		int i = 0;
	for (; i < this->GetCurPath().size(); ++i)
	{
		auto wp = this->GetCurPath().at(i);

		float dist = static_cast<float>((wp->GetPos() - trans_comp->_position).length());
		if (dist < closest_wp.second)
		{
			closest_wp.first = i;
			closest_wp.second = dist;
		}
	}

	this->cur_route_wp_index = i;	//wp index to go to will be the closest one
}
