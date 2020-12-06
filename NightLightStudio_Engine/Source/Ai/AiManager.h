#include "../Core/MySystem.h"
#include "WayPointManager.h"
#include "../Component/ComponentNavigator.h"
#include "../Component/ComponentPath.h"
#include <chrono>

namespace NS_AI
{
	class AiManager : public MySystem, public Singleton<AiManager>
	{
		WayPointManager wp_man;
		SystemMessaging::SystemReceiver r;

		//for dt
		std::chrono::system_clock::time_point timeThisRound;
		std::chrono::system_clock::time_point timeLastRound;
		std::chrono::system_clock::time_point stepTime;

		std::vector<Entity> Obstacle_list;
	protected:
		friend Singleton<AiManager>;

		AiManager()
		{}
		~AiManager() {}

	public:

		void HandleMsg(MessageTogglePlay& mst);
		void Init();
		void GameInit();


		void Update();

		void Exit()
		{
			DestroyInstance();
		}

		void WalkTowards(NavigatorComponent* nav_comp, NlMath::Vec3 my_pos, NlMath::Vec3 target_position)
		{
			nav_comp;
			if (wp_man.GetWayPointNumber())	//If there is way point
			{
				//nav_comp->SetCurrentPath(wp_man.AstarPathFinding(my_pos, target_position));
			}
			else
			{
				LocalVector<WayPoint*> temp_wp_list;
				wp_man.InsertWayPoint(my_pos, (my_pos - target_position).length());
				//nav_comp->SetCurrentPath
			}
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

		WayPoint& InsertWayPoint(NlMath::Vector3D position, float radius, int _ent_id = -1, LocalVector<Edges*>* _edges_list = nullptr)
		{
			wp_man.InsertWayPoint(position, radius, _ent_id, _edges_list);
		}

		//Select closest way point to move to
		void FindClosestWP(NavigatorComponent* nav_comp, TransformComponent* trans_comp)
		{
			std::pair<int, float> closest_wp = std::make_pair<int, float>(-1, std::numeric_limits<float>::max());
			
			int i = 0;
			for (; i < nav_comp->cur_path.size(); ++i)
			{
				auto wp = nav_comp->cur_path.at(i);

				float dist = static_cast<float>((wp->_position - trans_comp->_position).length());
				if (dist < closest_wp.second)
				{
					closest_wp.first = i;
					closest_wp.second = dist;
				}
			}

			nav_comp->cur_wp_index = i;	//wp index to go to will be the closest one
		}


		//=============Getter / Setter=================//
		/*
		void SetSpeed(float spd, NavigatorComponent* nav_comp)
		{
			nav_comp->speed = spd;
		}

		void SetIsFollowing(bool fol, NavigatorComponent* nav_comp)
		{
			//if (fol && !nav_comp->isFollowing) //if going from false to true
				//FindClosestWP(nav_comp, )

			nav_comp->isFollowing = fol;
		}

		bool GetIsPaused(NavigatorComponent* nav_comp)
		{
			return nav_comp->isPaused;
		}

		void SetIsPaused(bool pau, NavigatorComponent* nav_comp)
		{
			nav_comp->isPaused = pau;
		}*/


	};

	static AiManager* SYS_AI = AiManager::GetInstance();
}