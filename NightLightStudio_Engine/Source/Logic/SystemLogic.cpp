#include "SystemLogic.h"
#include "..\Mono\MonoWrapper.h" // Mono functions wrapped
// #include "..\Mono\Binding\Binder.h" // Bind C++ to C# functions

#include "../Input/SystemInput.h" // For testing
#include "CScripts/AllScripts.h"

namespace NS_LOGIC
{

  void SystemLogic::Load()
  {
    MonoWrapper::InitMono();
    MonoWrapper::ReloadScripts();
  }

  void SystemLogic::Init()
  {
    //Start up Scripting system??
    SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ReloadScripts", SystemInput_ns::IKEY_END, "ScriptReload", SystemInput_ns::OnRelease, [this]()
      {
        //Only if mouse wheel + alt button is pressed, camera will move.
        //NO CAMERA SPEED AS IT IS TOO FAST FOR FORWARD MOVEMENT
        if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IKEY_END))
        {
          printf("Recompiling...\n");
          MonoWrapper::ReloadScripts();
        }
      });
  }

  void SystemLogic::GameLoad()
  {
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      myComp->_pScript = AllScripts::Construct(myComp->_sName.toString());
    }
  }

  void SystemLogic::GameInit()
  {
    // MonoBind::Bind();
    // Init CS
    // MonoMethod* m_Init = MonoWrapper::GetObjectMethod("Init", "UniBehaviour");
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      myComp->_pScript->Init();
    }
  }

  void SystemLogic::Update()
  {
    //Run Script?
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      myComp->_pScript->Update();
    }
  }

  void SystemLogic::FixedUpdate()
  {

  }

  void SystemLogic::GameExit()
  {
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      myComp->_pScript->Exit();
    }
  }

  void SystemLogic::Free()
  {
  }

  void SystemLogic::Exit()
  {
    DestroyInstance();
  }

  void SystemLogic::OnCollisionEnter(Entity _obj1, Entity _obj2)
  {

  }

  void SystemLogic::OnCollisionStay(Entity _obj1, Entity _obj2)
  {

  }

  void SystemLogic::OnCollisionExit(Entity _obj1, Entity _obj2)
  {

  }

  void SystemLogic::OnTriggerEnter(Entity _obj1, Entity _obj2)
  {

  }

  void SystemLogic::OnTriggerStay(Entity _obj1, Entity _obj2)
  {

  }

  void SystemLogic::OnTriggerExit(Entity _obj1, Entity _obj2)
  {

  }
}
