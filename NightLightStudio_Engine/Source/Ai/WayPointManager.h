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

//Circular way point as of now
struct WayPoint 					
{
	bool isActive;         			//True for usual, false for when obstacle contains the point
	//NlMath::Vector3D position;
	//NlMath::Vector3D radius;	    // 0 for none circular waypoint
	SphereCollider sphere_col;

	//float cost;  //<-self inserted cost	
	//->std::map<CV, int> cost_var;		//map of variable that affect the cost
	std::pair<CV, int> distance_var;
	std::pair<CV, int> self_define_var;

	//std::vector<std::shared_ptr<Edges>> connected_edges;	
	LocalVector<int> connected_edges;	//Keep track of connecting edges

	NlMath::Vec3& GetPos()
	{
		return sphere_col.center;
	}

	float& GetRad()
	{
		return sphere_col.radius;
	}

	WayPoint()
		:isActive{true}
		, sphere_col
		{ 
			NlMath::Vector3D{0.0f,0.0f,0.0f},
			1.0f
		}
		, distance_var{ CV::Distance, 0.0f }
		, self_define_var{ CV::SelfDefine, 1.0f }
	{

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
				edges_list.push_back(Edges());
				Edges& new_edge = edges_list.at(edges_list.size()-1);
				new_edge.wp1 = &inserted_wp;
				new_edge.wp2 = &cur_wp;
				new_edge.distance_var.second = dist;
				
				_edges_list->push_back(&new_edge);

				//edges_list.push_back(new_edge);
			}
		}
		return inserted_wp;
	}

	WayPoint& InsertWayPoint(NlMath::Vector3D position, float radius, LocalVector<Edges*>* _edges_list = nullptr)
	{
		WayPoint wp;
		wp.sphere_col.colliderPosition = position;
		wp.sphere_col.radius = radius;
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

	LocalVector<WayPoint*> AstarPathFinding(NlMath::Vector3D start_point,
													NlMath::Vector3D end_point)
	{
		//1. Find collided waypoint for start and end
		//2. Start from starting waypoint, loop through each the edges conected to find the paths to the end
		//3. Choose the best route using the accumulated cost for each path


	
	};
};