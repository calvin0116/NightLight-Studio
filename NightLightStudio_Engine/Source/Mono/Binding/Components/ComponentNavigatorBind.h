#pragma once
#include "../../../Component/ComponentNavigator.h"
#include <mono/metadata/object.h>

namespace ComponentNavigatorBind
{
  float get_Speed_Internal(NavComponent* trans);
  void set_Speed_Internal(NavComponent* trans, float val);

  bool get_IsFollowing_Internal(NavComponent* trans);
  void set_IsFollowing_Internal(NavComponent* trans, bool val);

  bool get_IsPaused_Internal(NavComponent* trans);
  void set_IsPaused_Internal(NavComponent* trans, bool val);

  int get_NavState_Internal(NavComponent* trans);
  void set_NavState_Internal(NavComponent* trans, int val);

  //Way point functions
  void toggle_WayPointActive_Internal(NavComponent* nav, int val, bool act);
  bool MoreThenOneWPActive_Internal(NavComponent* nav);
  void GoToNextWp_Internal(NavComponent* nav);

  float getDistFromCurPathWP_Internal(NavComponent* nav);
  float getDistFromPrevPathWP_Internal(NavComponent* nav);

  bool IsAroundWP_Internal(NavComponent* nav);
  bool IsWithinCirclingRange_Internal(NavComponent* nav);

  MonoObject* get_Dir_Internal(NavComponent* nav);

  void BindComponentNavigator();
}
