#include "../Math/Vector.h"

#include "../Collision/CollisionMathLib.h"
#include "../Component/ComponentCollider.h"

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

	//float cost;               //<-self inserted cost	
	std::map<CV, int> cost_var;
	//std::vector<std::shared_ptr<Edges>> connected_edges;	
	LocalVector<int> connected_edges;	//Keep track of connecting edges
};

//Edges that connects two points
struct Edges
{
	bool isActive; 							//True for usual, false for when obstacle is obstructing
	std::shared_ptr<WayPoint> wp1, wp2;
	std::map<CV, int> cost_var;				//map of variable that affect the cost
	//float length;
	float total_cost;		//Depends on length only as of now

	float GetCost() { return total_cost; };
	float CalcCost() 
	{
		total_cost = 0;
		for (auto var : cost_var)
		{
			total_cost += var.second;
		}
	};
};

class WayPointManager
{
	//from assignment: NavMesh
	std::vector<std::shared_ptr<WayPoint>> waypoint_list;
	std::vector<std::shared_ptr<Edges>> edges_list;
	//std::vector<Collder*> obstacle_list;

	//Std::vector<NavMeshAgent*> navmesh_agent_list;
	bool isActive;
public:
	void InsertWayPoint(WayPoint& wp) {
		//0 way point
		waypoint_list.push_back(std::shared_ptr<WayPoint>(new WayPoint(wp)));

		for (auto cur_wp : waypoint_list)
		{
			float dist = NlMath::Sphere_SphereCollision(cur_wp->sphere_col, wp.sphere_col).length();
			if (!dist)
			{
				Edges* new_edge = new Edges;
				new_edge->wp1 = waypoint_list.back();
				new_edge->wp2 = cur_wp;
				new_edge->cost_var[CV::Distance] = dist;

				edges_list.push_back(std::shared_ptr<Edges>(new_edge));
			}
		}
	}

	void InsertWayPoint(NlMath::Vector3D position, float radius)
	{
		WayPoint wp;
		wp.sphere_col.colliderPosition = position;
		wp.sphere_col.radius = radius;
		//wp.radius = radius;
		
		InsertWayPoint(wp);
	};


	//Void InsertObstacle(AABB aabb collider);

	void RemoveWayPoint(int index) {};
	//Void RemoveObstacle(int index);

	void Update() {}; //Check for both edges and waypoint if they have been obstructed

	std::vector<NlMath::Vector3D> AstarPathFinding() {};
};