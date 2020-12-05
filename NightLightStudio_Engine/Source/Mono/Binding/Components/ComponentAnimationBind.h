#pragma once
#include "../../../Component/ComponentAnimation.h"
#include <mono/metadata/object.h>

namespace ComponentAnimationBind
{
  void PlayAnimation_Internal(ComponentAnimation* anim, MonoString* name, bool loop, double startFrame, double endFrame);
  void PauseAnimation_Internal(ComponentAnimation* anim);
  void ResumeAnimation_Internal(ComponentAnimation* anim);
  void StopAnimation_Internal(ComponentAnimation* anim);
  bool IsFinished_Empty_Internal(ComponentAnimation* anim);
  bool IsFinished_Internal(ComponentAnimation* anim, MonoString* name);

  void BindComponentAnimation();
}
