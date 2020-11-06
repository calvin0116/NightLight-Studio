#include "SystemLogic.h"
#include "..\Mono\MonoWrapper.h" // Mono functions wrapped
// #include "..\Mono\Binding\Binder.h" // Bind C++ to C# functions

#include "../Input/SystemInput.h" // For testing
#include "CScripts/AllScripts.h"

namespace NS_LOGIC
{
  // Variable to decide whether to run function
  bool SystemLogic::_isPlaying = false;
  bool SystemLogic::_Loaded = false;
  bool SystemLogic::_Inited = false;

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

    SYS_INPUT->GetSystemKeyPress().CreateNewEvent("PlayLogic", SystemInput_ns::IKEY_NUMPAD_9, "PlayLogic", SystemInput_ns::OnRelease, [this]()
      {
        //Only if mouse wheel + alt button is pressed, camera will move.
        //NO CAMERA SPEED AS IT IS TOO FAST FOR FORWARD MOVEMENT
        if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IKEY_NUMPAD_9))
        {
          _isPlaying = !_isPlaying;
          std::cout << "Logic Playing: " << _isPlaying << std::endl;
        }
      });
  }

  void SystemLogic::GameLoad()
  {
    if (!_isPlaying)
      return;
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      myComp->_pScript = AllScripts::Construct(myComp->_sName.toString());
      myComp->_pScript->SetEntity(G_ECMANAGER->getEntity(itr).getId());
    }
  }

  void SystemLogic::GameInit()
  {
    if (!_isPlaying)
      return;
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
    if (!_isPlaying)
      return;
    if (!_Loaded)
    {
      _Loaded = true;
      GameLoad();
    }
    if (!_Inited)
    {
      _Inited = true;
      GameInit();
    }
    ////Run Script?
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
    if (!_isPlaying)
      return;
  }

  void SystemLogic::GameExit()
  {
    if (!_isPlaying)
      return;
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      myComp->_pScript->Exit();
      // Delete memory
      delete myComp->_pScript;
      myComp->_pScript = nullptr;
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
    if (!_isPlaying)
      return;
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      Entity entity = G_ECMANAGER->getEntity(itr);
      if (entity.getId() == _obj1.getId())
        myComp->_pScript->OnCollisionEnter(_obj2.getId());
      else if (entity.getId() == _obj2.getId())
        myComp->_pScript->OnCollisionEnter(_obj1.getId());
    }
  }

  void SystemLogic::OnCollisionStay(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying)
      return;
  }

  void SystemLogic::OnCollisionExit(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying)
      return;
  }

  void SystemLogic::OnTriggerEnter(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying)
      return;
  }

  void SystemLogic::OnTriggerStay(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying)
      return;
  }

  void SystemLogic::OnTriggerExit(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying)
      return;
  }
}
