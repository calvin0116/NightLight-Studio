#pragma once
#include "../Math/Vector.h"


//#include "../Component/ComponentCollider.h"
#include "../Component/ComponentManager.h"
#include "../Component/Components.h"
#include "../Component/ComponentWayPoint.h"

#include "AiGraph.h"
#include <memory>
#include <unordered_map>
namespace NS_AI
{

	//************ Main Class***********************//
	class WayPointManager
	{
		//from assignment: NavMesh.
		LocalVector<WayPoint*> waypoint_list;			//List of object

		LocalVector<Edge,256> mem_edges_list;
		LocalVector<Edge*,256> edges_list;
		std::vector<ComponentCollider*> obstacle_list;

		//LocalVector<WayPointMapComponent> wpm_list;

		//Std::vector<NavMeshAgent*> navmesh_agent_list;
		bool isActive;
		bool runtimeUpdate;

		/*
		//Costing for single edge
		float CalcCost(Edges& edge)
		{
			edge.total_cost = 0;
			std::map<CV, int> cost_var;
			cost_var.insert(edge.distance_var);
			cost_var.insert(edge.self_define_var);
			for (auto var : cost_var)
			{
				edge.total_cost += var.second;
			}
		};

		//Length for single edge
		float CalcLength(Edges& edge)
		{
			edge.distance_var.second = (edge.wp1->sphere_col.center - edge.wp2->sphere_col.center).length();
		};
		*/
	public:
		~WayPointManager()
		{
			/*
			for (WayPoint* wp : waypoint_list)
			{
				delete wp;
			}*/
			/*
			for (Edges* edge : edges_list)
			{
				delete edge;
			}*/
		}

		//void RemoveWayPoint(int index) {};
		//Void RemoveObstacle(int index);
		void GameLoad();
		void GameInit();
		void Update(); //Check for both edges and waypoint if they have been obstructed


		//======Inserting circular way point======//

		void InsertWayPoint(WayPoint* wp, LocalVector<WayPoint*>* _wp_list = nullptr, LocalVector<Edge*, 256>* _edges_list = nullptr) {
			LocalVector<Edge*,256>* loc_edge_list;

			if (_edges_list != nullptr)
				loc_edge_list = _edges_list;
			else
				loc_edge_list = &edges_list;

			LocalVector<WayPoint*>* loc_wp_list;
			if (_wp_list != nullptr)
				loc_wp_list = _wp_list;
			else
				loc_wp_list = &waypoint_list;

			//waypoint_list.push_back(std::shared_ptr<WayPoint>(new WayPoint(wp)));
			loc_wp_list->push_back(wp);

			if (loc_wp_list->size() < 2)
				return;

			WayPoint* inserted_wp = wp;// loc_wp_list->back();
			for (auto cur_wp : *loc_wp_list)
			{
				bool isObstructed = false;
				for (auto obs : obstacle_list)
				{
					if (NlMath::RayToAABB(obs->collider.aabb, cur_wp->GetPos(), inserted_wp->GetPos()))
					{
						isObstructed = true;
					}
				}
				if (!isObstructed)
				{
					mem_edges_list.push_back(Edge(inserted_wp, cur_wp));
					loc_edge_list->push_back(&mem_edges_list.back());
					/*
					Edge* new_edge = loc_edge_list->at(edges_list.size() - 1);
					new_edge->wp1 = inserted_wp;
					new_edge->wp2 = cur_wp;*/
					//  new_edge->distance_var.second = dist;
				}
				/*
				float dist = NlMath::Sphere_SphereCollision(cur_wp.sphere_col, wp.sphere_col).length();
				if (!dist)
				{
					//Edges creation
					edges_list.push_back(Edges());
					Edges& new_edge = edges_list.at(edges_list.size() - 1);
					new_edge.wp1 = &inserted_wp;
					new_edge.wp2 = &cur_wp;
					new_edge.distance_var.second = dist;


					//Waypoint update
					cur_wp.connected_edges.push_back(&new_edge);
					inserted_wp.connected_edges.push_back(&new_edge);
					_edges_list->push_back(&new_edge);
				}
				*/
			}
			//return inserted_wp;
		}
		/*
		WayPoint& InsertWayPoint(NlMath::Vector3D position, float radius, int _ent_id = -1, LocalVector<Edges*>* _edges_list = nullptr)
		{
			WayPoint wp;
			wp.sphere_col.colliderPosition = position;
			wp.sphere_col.radius = radius;
			//wp.ent_id = _ent_id;
			//wp.radius = radius;

			return InsertWayPoint(wp, _edges_list);
		};


		//Update all edges
		float UpdateAllEdge()
		{
			for (auto& edge : edges_list)
			{
				CalcLength(edge);
				CalcCost(edge);
			}
		};*/
		void InsertObstacle(ComponentCollider* col_ptr)
		{
			obstacle_list.push_back(col_ptr);
		};

		void ClearObstacle()
		{
			obstacle_list.clear();
		};

		float Heuristic(NlMath::Vector3D end_point)
		{
			// -- Returns the furthest point from all the points to the end point
			float result = 0;
			for (WayPoint* wp : waypoint_list)
			{
				float sqrtlen = (wp->GetPos() - end_point).sqrtlength();
				if (sqrtlen > result)
					result = sqrtlen;
			}
			return result;
		}

		bool CheckEdgeWithObsticles(WayPointComponent* wp1, WayPointComponent* wp2);
		void BakeEdge();

		//Take in the path and edges to traverse
		//Return if path has reached the end and the resulting path and its value
		//cur_path: int -> next wp , std::vector<WayPoint*> -> path to next_wp
		//return: 
		/*
		std::pair<bool, std::pair<int, std::vector<WayPoint*>> > CheckPath(std::pair<int, std::vector<WayPoint*>> cur_path, Edge* connected_edges)
		{


		}*/

		//=============================Getter/Setter=====================================//
		int GetWayPointNumber()
		{
			return (int)waypoint_list.size();
		}

	};
}