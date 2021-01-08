#pragma once
#include "../Math/Vector.h"

#include "../Collision/CollisionMathLib.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentManager.h"

#include <memory>
#include <unordered_map>
namespace NS_AI
{
	struct WayPoint;
	struct Edges;

	typedef enum class COST_VARIABLE {
		G_DIST = 0,
		H_DIST,
		SELF_DEFINE,
	}CV;

	const float dist_multiper = 1.0f;
	const float self_define_multiplier = 1.0f;

	//Circular way point as of now
	struct WayPoint
	{
		int wp_id;											
		bool isActive;         					//True for usual, false for when obstacle contains the point
		//NlMath::Vector3D position;
		//NlMath::Vector3D radius;				// 0 for none circular waypoint
		SphereCollider sphere_col;

		//float cost;  //<-self inserted cost	
		//->std::map<CV, int> cost_var;			//map of variable that affect the cost
		std::pair<CV, float> h_dist_var;		//Heuristic distance away from the end point
		std::pair<CV, float> g_dist_var;		//Heuristic distance away from the start point
		std::pair<CV, float> self_define_var;

		//std::vector<std::shared_ptr<Edges>> connected_edges;	
		LocalVector<Edges*> connected_edges;		//Keep track of connecting edges
		WayPoint* parent;

		//=== Constructor===//  
		WayPoint() //ent_id{-1},
			:isActive{ true }
			, sphere_col
			{
				NlMath::Vector3D{0.0f,0.0f,0.0f},
				1.0f
			}
			, g_dist_var{ CV::G_DIST, 0.0f }
			, h_dist_var{ CV::H_DIST, 0.0f }
			, self_define_var{ CV::SELF_DEFINE, 1.0f }
			, parent{nullptr}
		{}
		//======Getter / Setter for alternate access ========//
		NlMath::Vec3& GetPos()
		{
			return sphere_col.center;
		}

		float& GetRad()
		{
			return sphere_col.radius;
		}

		float GetFCost()
		{
			return g_dist_var.second + h_dist_var.second + self_define_var.second;
		}

	};

	//Edges that connects two points
	struct Edges
	{
		bool isActive; 							//True for usual, false for when obstacle is obstructing
		WayPoint* wp1, * wp2;
		//std::map<CV, int> cost_var;				//map of variable that affect the cost
		//std::pair<CV, float> distance_var;
		//std::pair<CV, float> self_define_var;
		//float length;
		float total_cost;		//Depends on length only as of now

		Edges()
			:isActive{ true }
			, wp1{ nullptr }
			, wp2{ nullptr }
			//, distance_var{ CV::Distance, 0.0f }
			//, self_define_var{ CV::SelfDefine, 1.0f }
			, total_cost{ 0.0f }
		{}

		float GetCost() { return total_cost; };
		NlMath::Vec3 DirVec() { return wp2->GetPos() - wp1->GetPos(); };
		
		float Magnitude() { 
			return DirVec().length();
		};

		WayPoint* NextWayPoint(WayPoint* other_wp)
		{
			if (wp1 == other_wp)
				return wp2;
			else if (wp2 == other_wp)
				return wp1;

			std::cout << "Edges with Missing Way Point" << std::endl;
			throw(1);
			return nullptr;
		}
	};

	class WayPointManager
	{
		//from assignment: NavMesh
		LocalVector<WayPoint> waypoint_list;
		LocalVector<Edges> edges_list;
		std::vector<ComponentCollider*> obstacle_list;

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
		//======Inserting circular way point======//
		/*
		WayPoint& InsertWayPoint(WayPoint wp, LocalVector<Edges*>* _edges_list = nullptr) {
			//0 way point
			//waypoint_list.push_back(std::shared_ptr<WayPoint>(new WayPoint(wp)));
			waypoint_list.push_back(wp);
			WayPoint& inserted_wp = waypoint_list.at(waypoint_list.size() - 1);
			for (auto cur_wp : waypoint_list)
			{
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
			}
			return inserted_wp;
		}

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

		//void RemoveWayPoint(int index) {};
		//Void RemoveObstacle(int index);

		void Update() {
			if(runtimeUpdate)
				//Check if edges has any obstacle inbetween them
				for (Edges& edge : edges_list)
				{
					for (ColliderComponent* obst : obstacle_list)
						if (NlMath::RayToAABB(obst->collider.aabb, edge.wp1->GetPos(), edge.wp2->GetPos()))
							edge.isActive = false;		//turn off something is inbetween
														//** alternate idea may be have a new waypoint around the obstacle to go around it
				}
		
		}; //Check for both edges and waypoint if they have been obstructed

		float Heuristic(NlMath::Vector3D end_point)
		{
			// -- Returns the furthest point from all the points to the end point
			float result = 0;
			for (WayPoint& wp : waypoint_list)
			{
				float sqrtlen = (wp.GetPos() - end_point).sqrtlength();
				if (sqrtlen > result)
					result = sqrtlen;
			}
			return result;
		}


		//Take in the path and edges to traverse
		//Return if path has reached the end and the resulting path and its value
		//cur_path: int -> next wp , std::vector<WayPoint*> -> path to next_wp
		//return: 
		std::pair<bool, std::pair<int, std::vector<WayPoint*>> > CheckPath(std::pair<int, std::vector<WayPoint*>> cur_path, Edges* connected_edges)
		{


		}
		//Search for best way point to 
		LocalVector<int> AstarWayPointFinding(WayPoint* start_wp, WayPoint* end_wp)
		{
			//LocalVector<WayPoint*> cur_path;
			//pair -> way point & f value 
			std::unordered_map<WayPoint*, int> openList;
			std::unordered_map<WayPoint*, int> closeList;

			//Insert starting way point as the first
			openList[start_wp] = 0;
			WayPoint* cur_wp = start_wp;

			//Loop till you hit the end point
			while (openList.find(end_wp)!= openList.end())
			{
				int cur_cost = INT_MAX;
				WayPoint* cur_best_wp = nullptr;
				WayPoint* prev_wp = cur_wp;

				//Get the best node with the least cost
				for (Edges* edge : cur_wp->connected_edges)
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


		LocalVector<int> AstarPathFinding(NlMath::Vector3D start_point,
			NlMath::Vector3D end_point,
			LocalVector<WayPoint*> way_points,
			int ent_id = -1)
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
		};

		//=============================Getter/Setter=====================================//
		int GetWayPointNumber()
		{
			return (int)waypoint_list.size();
		}

	};
}