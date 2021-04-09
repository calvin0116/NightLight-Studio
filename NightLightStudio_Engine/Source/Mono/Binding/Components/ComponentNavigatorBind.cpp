#include "ComponentNavigatorBind.h"
#include "../../MonoWrapper.h"


namespace ComponentNavigatorBind
{
  void BindComponentNavigator()
  {
    MonoWrapper::BindClassFunction(get_Speed_Internal, "get_Speed_Internal",
      "Navigator");
    MonoWrapper::BindClassFunction(set_Speed_Internal, "set_Speed_Internal",
      "Navigator");

    MonoWrapper::BindClassFunction(get_IsFollowing_Internal, "get_IsFollowing_Internal",
      "Navigator");
    MonoWrapper::BindClassFunction(set_IsFollowing_Internal, "set_IsFollowing_Internal",
      "Navigator");

    MonoWrapper::BindClassFunction(get_IsPaused_Internal, "get_IsPaused_Internal",
      "Navigator");
    MonoWrapper::BindClassFunction(set_IsPaused_Internal, "set_IsPaused_Internal",
      "Navigator");

    MonoWrapper::BindClassFunction(get_NavState_Internal, "get_NavState_Internal",
        "Navigator");
    MonoWrapper::BindClassFunction(set_NavState_Internal, "set_NavState_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(toggle_WayPointActive_Internal, "toggle_WayPointActive_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(MoreThenOneWPActive_Internal, "MoreThenOneWPActive_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(GoToNextWp_Internal, "GoToNextWp_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(get_Dir_Internal, "get_Dir_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(getDistFromCurPathWP_Internal, "getDistFromCurPathWP_Internal",
        "Navigator");
    MonoWrapper::BindClassFunction(getDistFromPrevPathWP_Internal, "getDistFromPrevPathWP_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(IsAroundWP_Internal, "IsAroundWP_Internal",
        "Navigator");
    MonoWrapper::BindClassFunction(IsWithinCirclingRange_Internal, "IsWithinCirclingRange_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(GetCurWpId_Internal, "GetCurWpId_Internal",
        "Navigator");

    MonoWrapper::BindClassFunction(GetActiveWpId_Internal, "GetActiveWpId_Internal",
        "Navigator");
   MonoWrapper::BindClassFunction(GetActiveWpIdListSize_Internal, "GetActiveWpIdListSize_Internal",
       "Navigator");
  }

  float get_Speed_Internal(NavComponent* nav)
  {
    return nav->speed;
  }

  void set_Speed_Internal(NavComponent* nav, float val)
  {
    nav->SetSpeed(val);
  }

  bool get_IsFollowing_Internal(NavComponent* nav)
  {
    return nav->isFollowing;
  }

  void set_IsFollowing_Internal(NavComponent* nav, bool val)
  {
    nav->SetIsFollowing(val);
  }

  bool get_IsPaused_Internal(NavComponent* nav)
  {
    return nav->GetIsPaused();
  }

  void set_IsPaused_Internal(NavComponent* nav, bool val)
  {
    nav->SetIsPaused(val);
  }
  int get_NavState_Internal(NavComponent* nav)
  {
      return (int)nav->nav_state;
  }
  void set_NavState_Internal(NavComponent* nav, int val)
  {
      nav->nav_state = (NAV_STATE)val;
  }

  void toggle_WayPointActive_Internal(NavComponent* nav, int val, bool act)
  {
      nav->ToggleWayPointActive(val, act);
  }
  bool MoreThenOneWPActive_Internal(NavComponent* nav)
  {
      return nav->MoreThenOneWPActive();
  }
  void GoToNextWp_Internal(NavComponent* nav)
  {
      nav->DecideOnNextWp();
  }
  float getDistFromCurPathWP_Internal(NavComponent* nav)
  {
      return nav->DistFromCurPathWP();
  }

  float getDistFromPrevPathWP_Internal(NavComponent* nav)
  {
      return nav->DistFromPrevPathWP();
  }

  bool IsAroundWP_Internal(NavComponent* nav)
  {
      return nav->IsAroundWP(1.0f);
  }
    
  bool IsWithinCirclingRange_Internal(NavComponent* nav)
  {
      return nav->IsWithinCirclingRange();
  }

  int GetCurWpId_Internal(NavComponent* nav)
  {
      return nav->GetCurPathWpEntityID();
  }

  int GetActiveWpId_Internal(NavComponent* nav, int index)
  {
      return nav->GetActiveWpId(index);
  }

  int GetActiveWpIdListSize_Internal(NavComponent* nav)
  {
      return nav->GetActiveWpIdListSize();
  }

  MonoObject* get_Dir_Internal(NavComponent* nav)
  {
      MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
      glm::vec3 dir = glm::normalize(nav->dir);
      MonoWrapper::SetObjectFieldValue(monoVec, "X", dir.x);
      MonoWrapper::SetObjectFieldValue(monoVec, "Y", dir.y);
      MonoWrapper::SetObjectFieldValue(monoVec, "Z", dir.z);
      return monoVec;
  }
}
