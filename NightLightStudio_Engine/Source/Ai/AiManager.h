#include "../Core/MySystem.h"
#include "WayPointManager.h"
#include "../Component/ComponentNavigator.h"
#include "../Component/ComponentPath.h"
class AiManager : public MySystem
{
	WayPointManager wp_man;

	/*
	void Update()
	{
		auto itr = G_ECMANAGER->begin<NavigatorComponent>();
		auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

	}*/
public:
	void WalkTowards(NavigatorComponent* nav_comp, NlMath::Vec3 my_pos ,NlMath::Vec3 target_position)
	{
		nav_comp->SetCurrentPath(wp_man.AstarPathFinding(my_pos, target_position));
	}

	void WalkTowards(int ent_id, NlMath::Vec3 target_position)
	{
		Entity ent = G_ECMANAGER->getEntity(ent_id);
		NavigatorComponent* nav_comp = ent.getComponent<NavigatorComponent>();
		TransformComponent* trans_comp = ent.getComponent<TransformComponent>();

		WalkTowards(nav_comp, trans_comp->_position, target_position);
	}

	//==================WayPointManager Fnc======================================//
	WayPoint& InsertWayPoint(WayPoint wp, LocalVector<Edges*>* _edges_list = nullptr)
	{
		wp_man.InsertWayPoint(wp, _edges_list);
	}

	WayPoint& InsertWayPoint(NlMath::Vector3D position, float radius, int _ent_id = -1,LocalVector<Edges*>* _edges_list = nullptr)
	{
		wp_man.InsertWayPoint(position, radius, _ent_id, _edges_list);
	}

};