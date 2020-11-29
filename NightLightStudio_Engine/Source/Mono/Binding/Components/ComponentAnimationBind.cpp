#include "ComponentAnimationBind.h"
#include "../../MonoWrapper.h"


namespace ComponentAnimationBind
{
  void BindComponentAnimation()
  {
    MonoWrapper::BindClassFunction(PlayAnimation_Internal, "PlayAnimation_Internal",
      "Animation");
    MonoWrapper::BindClassFunction(PauseAnimation_Internal, "PauseAnimation_Internal",
      "Animation");
    MonoWrapper::BindClassFunction(ResumeAnimation_Internal, "ResumeAnimation_Internal",
      "Animation");
    MonoWrapper::BindClassFunction(StopAnimation_Internal, "StopAnimation_Internal",
      "Animation");
  }

  void PlayAnimation_Internal(ComponentAnimation* anim, MonoString* name, bool loop)
  {
    anim->PlayAnimation(MonoWrapper::ToString(name), loop);
  }

  void PauseAnimation_Internal(ComponentAnimation* anim)
  {
    anim->PauseAnimation();
  }

  void ResumeAnimation_Internal(ComponentAnimation* anim)
  {
    anim->ResumeAnimation();
  }

  void StopAnimation_Internal(ComponentAnimation* anim)
  {
    anim->StopAnimation();
  }

}
