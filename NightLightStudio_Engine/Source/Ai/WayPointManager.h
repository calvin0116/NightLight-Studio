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
	SphereCollider* sphere_col;

	//float cost;               //<-self inserted cost	
	std::map<CV, int> cost_var;
	std::shared_ptr<Edges> connected_edges;
};

//Edges that connects two points
struct Edges
{
	bool isActive; 							//True for usual, false for when obstacle is obstructing
	std::shared_ptr<WayPoint> wp1, wp2;
	std::map<CV, int> cost_var;				//map of variable that affect the cost
	//float length;
	//float cost;		//Depends on length only as of now

	float GetCost() {};
	float CalcCost() {};
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
		if (!waypoint_list.size())
			waypoint_list.push_back(std::shared_ptr<WayPoint>(new WayPoint(wp)));

		for (auto wp : waypoint_list)
		{
		
		}
	}
	
	void InsertWayPoint(NlMath::Vector3D position, NlMath::Vector3D radius)
	{
		WayPoint wp;
		wp.position = position;
		wp.radius = radius;
		InsertWayPoint(wp);
	};
	

	//Void InsertObstacle(AABB aabb collider);

	void RemoveWayPoint(int index);
	//Void RemoveObstacle(int index);

	Void Update(); //Check for both edges and waypoint if they have been obstructed

	std::vector<Vector3> AstarPathFinding();
};