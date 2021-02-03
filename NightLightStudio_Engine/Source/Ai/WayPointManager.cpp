#include "WayPointManager.h"

#include "../Graphics/GraphicsSystem.h"
//#include "../Component/Components.h"

namespace NS_AI {



	void WayPointManager::Init()
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

	//Search for best way point to 
	inline LocalVector<int> WayPointManager::AstarWayPointFinding(WayPoint* start_wp, WayPoint* end_wp)
	{
		//LocalVector<WayPoint*> cur_path;
		//pair -> way point & f value 
		std::unordered_map<WayPoint*, int> openList;
		std::unordered_map<WayPoint*, int> closeList;

		//Insert starting way point as the first
		openList[start_wp] = 0;
		WayPoint* cur_wp = start_wp;

		//Loop till you hit the end point
		while (openList.find(end_wp) != openList.end())
		{
			int cur_cost = INT_MAX;
			WayPoint* cur_best_wp = nullptr;
			WayPoint* prev_wp = cur_wp;

			//Get the best node with the least cost
			for (Edge* edge : cur_wp->connected_edges)
			{
				WayPoint* next_wp = edge->NextWayPoint(cur_wp);
				if (next_wp->GetFCost() < cur_cost)
				{
					cur_cost = next_wp->GetFCost();
					cur_best_wp = next_wp;
				}
			}
			//Check if best wp is the end;
			if (cur_best_wp == end_wp)
			{
				cur_best_wp->parent = prev_wp;
				openList[cur_best_wp] = openList[cur_best_wp] + cur_best_wp->GetFCost();
				break;
			}
			else //Put into close list
			{
				//closeList[cur_best_wp]
			}

		}

		//Loop backwards to find the best path
		LocalVector<int> best_path;
		while (cur_wp != start_wp)
		{

		}
		//return cur_path;
		return best_path;
	}
	inline LocalVector<int> WayPointManager::AstarPathFinding(NlMath::Vector3D start_point, NlMath::Vector3D end_point, LocalVector<WayPoint*> way_points, int ent_id)
	{
		//0.1 Setup for Astar
		int indexing = 0;
		for (WayPoint*& _wp : way_points)
		{
			_wp->wp_id = indexing;
			++indexing;
		}

		//1. Find collided waypoint for start and end
		WayPoint* start_wp = nullptr, * end_wp = nullptr;
		LocalVector<int> return_wp;

		if (ent_id == -1)
		{
			for (WayPoint*& wp : way_points)
			{
				//Check if point is in circle
				if ((wp->GetPos() - start_point).sqrtlength() < wp->GetRad() * wp->GetRad())
				{
					start_wp = wp;
				}
			}

			for (WayPoint*& wp : way_points)
			{
				//Check if point is in circle
				if ((wp->GetPos() - end_point).sqrtlength() < wp->GetRad() * wp->GetRad())
				{
					end_wp = wp;
					break;
				}
			}
		}

		//1.1 Check for being in the same way point
		if (start_wp == end_wp)
		{
			return_wp.push_back(end_wp->wp_id);
			return return_wp;
		}
		else if (start_wp == nullptr || end_wp == nullptr)
		{
			std::cout << "Starting / Ending way point not found " << std::endl;
		}

		//2. Start from starting waypoint, loop through each the edges conected to find the paths to the end
		return AstarWayPointFinding(start_wp, end_wp);
		/*
		std::pair<int, std::vector<WayPoint*>> wp_list;
		std::vector< std::pair<int, std::vector<WayPoint*> > > route_to_end;
		wp_list.second.push_back(start_wp);

		while (true)
		{
		//Go to the nodes connectted to the edges
		//for (Edges* edge : wp_list.second.back()->connected_edges)
		{
		//A method that take in
		//std::pair<bool, std::pair<int, std::vector<WayPoint*>>> result = CheckPath(wp_list, edge);
		}

		}

		//3. Choose the best route using the accumulated cost for each path
		*/
	}
}