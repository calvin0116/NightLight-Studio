#include "ComponentNavigator.h"
#include "../Component/ComponentManager.h"

float ComponentNavigator::DistFromCurPathWP()
{
	TransformComponent* trans_comp = G_ECMANAGER->getEntity(this).getComponent<TransformComponent>();
	return glm::distance(GetCurPathWp()->GetPos(),trans_comp->_position);
}

float ComponentNavigator::DistFromPrevPathWP()
{
	TransformComponent* trans_comp = G_ECMANAGER->getEntity(this).getComponent<TransformComponent>();
	return  glm::distance(GetPrevPathWp()->GetPos(), trans_comp->_position);
}

void ComponentNavigator::InitPath()
{
	Entity ent = G_ECMANAGER->getEntity(this);
	trans = ent.getComponent<TransformComponent>();
	rb = ent.getComponent<RigidBody>();

	if (cur_wp_path == nullptr)
	{
		if (!wp_path_ent_name.empty())
		{
			Entity path_ent = G_ECMANAGER->getEntityUsingEntName((std::string)wp_path_ent_name);
			if (path_ent.getId() != -1)
				cur_wp_path = path_ent.getComponent<WayPointMapComponent>();
			else
			{
				TracyMessageL("ComponentNavigator::InitPath : No entity have been found");
				SPEEDLOG("ComponentNavigator::InitPath : No entity have been found");
				return;
			}
		}
		else
		{
			TracyMessageL("ComponentNavigator::InitPath : No entity name have been found");
			SPEEDLOG("ComponentNavigator::InitPath : No entity name have been found");
			return;
		}
		//if way point path entity does not exist
		if (cur_wp_path == nullptr)
			return;
	}

	cur_route_wp_index = 0;
	prev_route_wp_index = 0;
	isPaused = false;

	switch (wp_creation_type)
	{
	case WPP_STANDARD:
	{
		path_indexes.clear();
		size_t wp_size = cur_wp_path->GetPath().size();
		for (size_t i = 0; i < wp_size; ++i)
			path_indexes.push_back(std::make_pair((int)i, true));
		break;
	}
	case WPP_REVERSE:
	{
		path_indexes.clear();
		size_t wp_size = cur_wp_path->GetPath().size() - 1;
		for (int i = static_cast<int>(wp_size); i >= 0; --i)
			path_indexes.push_back(std::make_pair(i, true));
		break;
	}
	case WPP_CUSTOM:	//Inserted beforehand
	{
		break;
	}
	}
	//SetNextWp(this);

	active_ent_id_list = GetActiveWpsId();	//Set up active ent list
}

inline void ComponentNavigator::GoToClosestWP()
{
	std::pair<int, float> closest_wp = std::make_pair<int, float>(-1, std::numeric_limits<float>::max());
	TransformComponent* trans_comp = G_ECMANAGER->getEntity(this).getComponent<TransformComponent>();

		int i = 0;
	for (; i < this->GetCurPath().size(); ++i)
	{
		auto wp = this->GetCurPath().at(i);

		float dist = glm::distance(wp->GetPos(), trans_comp->_position);
		if (dist < closest_wp.second)
		{
			closest_wp.first = i;
			closest_wp.second = dist;
		}
	}
	this->prev_route_wp_index = cur_route_wp_index;
	this->cur_route_wp_index = i;	//wp index to go to will be the closest one
}

bool ComponentNavigator::IsAroundWP(float proximity)
{
	float len = glm::length(dir);

	return ((len - size_in_rad) < (FLT_EPSILON + proximity));
}

bool ComponentNavigator::IsWithinCirclingRange()
{
	float len = glm::length(dir);

	return ((len - circuling_rad) < FLT_EPSILON);
}
