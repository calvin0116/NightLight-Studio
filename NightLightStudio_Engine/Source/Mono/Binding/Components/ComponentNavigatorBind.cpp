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
}
