#include "AllBindings.h"
#include "Binder.h"


namespace MonoBind
{
  void BindCustomTypes()
  {      
    //Vector3DBind::BindVector3D();
    //Color4Bind::BindColor4();
  }

  void BindComponentsGetSet()
  {
    //ECSBind::BindECS();
    //ObjectInfoBind::BindObjectInfo();
    //BoxColliderBind::BindBoxCollider();
    //RigidbodyBind::BindRigidbody();
    //AIBind::BindAI();
    //AudioSourceBind::BindAudioSource();
    //SpriteRendererBind::BindSpriteRenderer();
    //AnimatorBind::BindAnimator();
    //CameraBind::BindCamera();
    //UISliderBind::BindUISlider();
    //UIButtonBind::BindUIButton();
    //GUITextBind::BindGUIText();
  }

  void Bind()
  {
    BindCustomTypes();
    BindComponentsGetSet();      
    //InputBind::BindInput();
    //DebugBind::BindDebug();
    AudioBind::BindAudio();
    //SceneMgrBind::BindSceneMgr();
    //RNGBind::BindRNG();
    //ApplicationBind::BindApplication();
  }
}