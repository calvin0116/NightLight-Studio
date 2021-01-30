#include "WayPointManager.h"

#include "../Graphics/GraphicsSystem.h"
#include "../Component/Components.h"

namespace NS_AI {



	void WayPointManager::GameInit()
	{

	}

	void WayPointManager::Update() {
		auto itr = G_ECMANAGER->begin<WayPointMapComponent>();
		auto itrEnd = G_ECMANAGER->end<WayPointMapComponent>();

		while (itr != itrEnd)
		{
			WayPointMapComponent* wpm_comp = reinterpret_cast<WayPointMapComponent*>(*itr);
			
			for (int i = 0; i < wpm_comp->WPSize() - 1; ++i)
			{
				for (int j = i + 1; j < wpm_comp->WPSize(); ++j)
				{
					NS_GRAPHICS::SYS_GRAPHICS->DrawLine(
						wpm_comp->GetPath().at(i)->GetPos(),
						wpm_comp->GetPath().at(j)->GetPos()
					);
				}
			}
			itr++;
		}
		/**
		if(runtimeUpdate)
		//Check if edges has any obstacle inbetween them
		for (Edges& edge : edges_list)
		{
		for (ColliderComponent* obst : obstacle_list)
		if (NlMath::RayToAABB(obst->collider.aabb, edge.wp1->GetPos(), edge.wp2->GetPos()))
		edge.isActive = false;		//turn off something is inbetween
		//** alternate idea may be have a new waypoint around the obstacle to go around it
		}
		*/
	}
}