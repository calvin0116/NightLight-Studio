#include "../Math/Vector.h"

#include "../Collision/CollisionMathLib.h"
#include "../Component/ComponentCollider.h"

#include "../Component/ComponentManager.h"

#include <memory>

struct WayPoint;
struct Edges;

typedef enum class COST_VARIABLE{
	Distance = 0,
	SelfDefine
}CV;

const float dist_multiper = 1.0f;
const float self_define_multiplier = 1.0f;

//Circular way point as of now
struct WayPoint 					
{
	int ent_id;								//Keep track of the owner of the way point			
	bool isActive;         					//True for usual, false for when obstacle contains the point
	//NlMath::Vector3D position;
	//NlMath::Vector3D radius;				// 0 for none circular waypoint
	SphereCollider sphere_col;

	//float cost;  //<-self inserted cost	
	//->std::map<CV, int> cost_var;			//map of variable that affect the cost
	std::pair<CV, int> distance_var;		//Heuristic distance away from the end point
	std::pair<CV, int> self_define_var;

	//std::vector<std::shared_ptr<Edges>> connected_edges;	
	LocalVector<Edges*> connected_edges;		//Keep track of connecting edges


	WayPoint()
		:isActive{ true }
		, sphere_col
	{
		NlMath::Vector3D{0.0f,0.0f,0.0f},
		1.0f
	}
		, distance_var{ CV::Distance, 0.0f }
		, self_define_var{ CV::SelfDefine, 1.0f }
	{

	}
	//======Getter / Setter for alternate access ========//
	NlMath::Vec3& GetPos()
	{
		return sphere_col.center;
	}

	float& GetRad()
	{
		return sphere_col.radius;
	}

};

//Edges that connects two points
struct Edges
{
	bool isActive; 							//True for usual, false for when obstacle is obstructing
	WayPoint* wp1, *wp2;
	//std::map<CV, int> cost_var;				//map of variable that affect the cost
	std::pair<CV, int> distance_var;
	std::pair<CV, int> self_define_var;
	//float length;
	float total_cost;		//Depends on length only as of now

	Edges()
		:isActive{true}
		, wp1{nullptr}
		, wp2{nullptr}
		, distance_var{CV::Distance, 0.0f}
		, self_define_var{ CV::SelfDefine, 1.0f }
		, total_cost{ 0.0f }
	{}

	float GetCost() { return total_cost; };
};

class WayPointManager 
{
	//from assignment: NavMesh
	LocalVector<WayPoint> waypoint_list;
	LocalVector<Edges> edges_list;
	//std::vector<Collder*> obstacle_list;

	//Std::vector<NavMeshAgent*> navmesh_agent_list;
	bool isActive;
public:
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
				Edges& new_edge = edges_list.at(edges_list.size()-1);
				new_edge.wp1 = &inserted_wp;
				new_edge.wp2 = &cur_wp;
				new_edge.distance_var.second = dist;


				//Waypoint update
				cur_wp.connected_edges.push_back(&new_edge);
				inserted_wp.connected_edges.push_back(&new_edge);
				//edges_list.push_back(new_edge);
			}
		}
		return inserted_wp;
	}

	WayPoint& InsertWayPoint(NlMath::Vector3D position, float radius,  int _ent_id = -1, LocalVector<Edges*>* _edges_list = nullptr)
	{
		WayPoint wp;
		wp.sphere_col.colliderPosition = position;
		wp.sphere_col.radius = radius;
		wp.ent_id = _ent_id;
		//wp.radius = radius;
		
		return InsertWayPoint(wp);
	};

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
	//Update all edges
	float UpdateAllEdge()
	{
		for (auto& edge : edges_list)
		{
			CalcLength(edge);
			CalcCost(edge);
		}
	};
	//Void InsertObstacle(AABB aabb collider);

	void RemoveWayPoint(int index) {};
	//Void RemoveObstacle(int index);

	void Update() {}; //Check for both edges and waypoint if they have been obstructed

	float Heuristic(NlMath::Vector3D end_point)
	{
		// -- Returns the furthest point from all the points to the end point
		float result = 0;
		for (WayPoint& wp : waypoint_list)
		{
			float sqrtlen = (wp.GetPos() - end_point).sqrtlength();
			wp.distance_var.second = sqrtlen;
			if (sqrtlen > result)
				result = sqrtlen;
		}
		return result;
	}

	//Take in the path and edges to traverse
	//Return if path has reached the end and the resulting path and its value
	std::pair<bool, std::pair<int, std::vector<WayPoint*>> > CheckPath(std::pair<int, std::vector<WayPoint*>> cur_path, Edges* connected_edges)
	{


	}

	LocalVector<WayPoint*> AstarPathFinding(NlMath::Vector3D start_point,
											NlMath::Vector3D end_point,
											int ent_id = -1)
	{
		//1. Find collided waypoint for start and end
		WayPoint* start_wp, *end_wp;
		LocalVector<WayPoint*> return_wp;
		
		if (ent_id == -1)
		{
			for (WayPoint& wp : waypoint_list)
			{
				//Check if point is in circle
				if ((wp.GetPos() - start_point).sqrtlength() < wp.GetRad() * wp.GetRad())
				{
					start_wp = &wp;
				}
			}

			for (WayPoint& wp : waypoint_list)
			{
				//Check if point is in circle
				if ((wp.GetPos() - end_point).sqrtlength() < wp.GetRad() * wp.GetRad())
				{
					end_wp = &wp;
					break;
				}
			}
		}

		//Check for being in the same way point
		if (start_wp == end_wp)
		{
			return_wp.push_back(end_wp);
			return return_wp;
		}

		//2. Start from starting waypoint, loop through each the edges conected to find the paths to the end
		
		std::pair<int, std::vector<WayPoint*>> wp_list;
		std::vector< std::pair<int, std::vector<WayPoint*> > > route_to_end;
		wp_list.second.push_back(start_wp);
		
		while (true)
		{
			//Go to the nodes connectted to the edges
			for (Edges* edge : wp_list.second.back()->connected_edges)
			{
				//A method that take in 
				std::pair<bool, std::pair<int, std::vector<WayPoint*>>> result = CheckPath(wp_list, edge);
			}

		}
		
		//3. Choose the best route using the accumulated cost for each path
		
	};




};