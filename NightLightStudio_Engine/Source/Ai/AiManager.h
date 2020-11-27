#include "../Core/MySystem.h"
#include "WayPointManager.h"
#include "../Component/ComponentNavigator.h"
#include "../Component/ComponentPath.h"

namespace NS_AI
{
	class AiManager : public MySystem, public Singleton<AiManager>
	{
		WayPointManager wp_man;
		SystemMessaging::SystemReceiver r;

	protected:
		friend Singleton<AiManager>;

		AiManager() {}
		~AiManager() {}

	public:

		void HandleMsg(MessageTogglePlay& mst)
		{
			if (mst.GetID() == "BeforePlay")
			{
				// Game started playing
				//TempSave();
				GameInit();
			}
		}

		void Init()
		{
			r.AttachHandler("BeforePlay", &AiManager::HandleMsg, this);
			//r.AttachHandler("AfterPlay", &SceneManager::HandleMsg, this);
		}

		void GameInit()
		{
			auto itr = G_ECMANAGER->begin<NavigatorComponent>();
			auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

			while (itr != itrEnd)
			{
				NavigatorComponent* navComp = reinterpret_cast<NavigatorComponent*>(*itr);
				navComp->InitPath();
				++itr;
			}

		}


		void Update()
		{
			auto itr = G_ECMANAGER->begin<NavigatorComponent>();
			auto itrEnd = G_ECMANAGER->end<NavigatorComponent>();

			while (itr != itrEnd)
			{
				TransformComponent* trans = G_ECMANAGER->getComponent<TransformComponent>(itr);
				//if(trans->_position - (*itr)

				++itr;
			}
		}

		void Exit()
		{
			DestroyInstance();
		}

		void WalkTowards(NavigatorComponent* nav_comp, NlMath::Vec3 my_pos, NlMath::Vec3 target_position)
		{
			if (wp_man.GetWayPointNumber())	//If there is way point
			{
				nav_comp->SetCurrentPath(wp_man.AstarPathFinding(my_pos, target_position));
			}
			else
			{
				LocalVector<WayPoint*> temp_wp_list;
				//wp_man.InsertWayPoint(my_pos, (my_pos - target_position).re)
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

	};

	static AiManager* SYS_AI = AiManager::GetInstance();
}