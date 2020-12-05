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
    MonoWrapper::BindClassFunction(IsFinished_Empty_Internal, "IsFinished_Empty_Internal",
      "Animation");
    MonoWrapper::BindClassFunction(IsFinished_Internal, "IsFinished_Internal",
      "Animation");
  }

  void PlayAnimation_Internal(ComponentAnimation* anim, MonoString* name, bool loop, double startFrame, double endFrame)
  {
    anim->PlayAnimation(MonoWrapper::ToString(name), loop, startFrame, endFrame);
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

  bool IsFinished_Empty_Internal(ComponentAnimation* anim)
  {
    return anim->IsFinished();
  }

  bool IsFinished_Internal(ComponentAnimation* anim, MonoString* name)
  {
    return anim->IsFinished(MonoWrapper::ToString(name));
  }
}
