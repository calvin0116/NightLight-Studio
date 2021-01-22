#pragma once

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

	//Waypoint class: Circular way point as of now
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
			, parent{ nullptr }
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

}