#include "WayPointManager.h"

#include "../Graphics/GraphicsSystem.h"
//#include "../Component/Components.h"

namespace NS_AI {



	void WayPointManager::GameLoad()
	{
		/*
		if (edges_list.size() == 0)
		{
			BakeEdge();
		}*/
	}

	void WayPointManager::GameInit()
	{

	}

	void WayPointManager::Update() {
#ifdef _EDITOR
		auto itr = G_ECMANAGER->begin<WayPointMapComponent>();
		auto itrEnd = G_ECMANAGER->end<WayPointMapComponent>();

		while (itr != itrEnd)
		{
			WayPointMapComponent* wpm_comp = reinterpret_cast<WayPointMapComponent*>(*itr);
			//TODO: 
			//Change draw line from wp to wp -> edges
			/*
			for (int i = 0; i < wpm_comp->WPSize() - 1; ++i)
			{
				for (int j = i + 1; j < wpm_comp->WPSize(); ++j)
				{
					
					NS_GRAPHICS::SYS_GRAPHICS->DrawLine(
						wpm_comp->GetPath().at(i)->GetPos(),
						wpm_comp->GetPath().at(j)->GetPos()
					);
				}
			}*/
			for (Edge* edge : wpm_comp->GetEdge())
			{
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(
					edge->wp1->GetPos(),
					edge->wp2->GetPos()
				);
			}
			itr++;
		}
#endif
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
	bool WayPointManager::CheckEdgeWithObsticles(WayPointComponent* wp1, WayPointComponent* wp2)
	{
		for (ColliderComponent* obst : obstacle_list)
			if (NlMath::RayToAABB(obst->collider.aabb, wp1->GetPos(), wp2->GetPos()))
			{
				return true;		//turn off something is inbetween
			}
		return false;
	}
	void WayPointManager::BakeEdge()
	{
		//TODO: 
		//CHECK Edge with obstacle to see if edge should be created
		//Button to Bake edges

		auto itr = G_ECMANAGER->begin<WayPointMapComponent>();
		auto itrEnd = G_ECMANAGER->end<WayPointMapComponent>();

		while (itr != itrEnd)
		{
			WayPointMapComponent* wpm_comp = reinterpret_cast<WayPointMapComponent*>(*itr);

			wpm_comp->GetEdge().clear();	//Clear edge to be rebuild

			auto itr_wp1 = wpm_comp->GetPath().begin();
			auto itr_wp2 = itr_wp1;
			auto itr_wpend = wpm_comp->GetPath().end();

			while (itr_wp1 != itr_wpend)
			{
				WayPointComponent* wpc_1 = reinterpret_cast<WayPointComponent*>(*itr_wp1);
				itr_wp2 = itr_wp1++;
				while (itr_wp2 != itr_wpend)
				{
					WayPointComponent* wpc_2 = reinterpret_cast<WayPointComponent*>(*itr_wp2);
					
					if (!CheckEdgeWithObsticles(wpc_1, wpc_2))
					{
						mem_edges_list.push_back(Edge(wpc_1, wpc_2));
						edges_list.push_back(&mem_edges_list.back());
						wpm_comp->GetEdge().push_back(&mem_edges_list.back());
					}

					++itr_wp2;
				}
			}
			itr++;
		}
	}
}