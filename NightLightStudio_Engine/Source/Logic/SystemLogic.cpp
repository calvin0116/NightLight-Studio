#include "SystemLogic.h"
#include "..\Mono\MonoWrapper.h" // Mono functions wrapped
#include "..\Mono\Binding\Binder.h" // Bind C++ to C# functions

#include "../Input/SystemInput.h" // For testing
#include "CScripts/AllScripts.h"

#define C_ENV

namespace NS_LOGIC
{
  // Variable to decide whether to run function
// Getting the bool needed
// #include "../IO/Json/Config.h"
// ->  CONFIG_DATA->GetConfigData().isPlaying;      //Play button toggling bool
// -> CONFIG_DATA->GetConfigData().engineRunning;
// -> CONFIG_DATA->GetConfigData().sceneRunning;
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

    // Attach handler
    r.AttachHandler("ScriptRequest", &SystemLogic::HandleMsg, this);
    r.AttachHandler("ApplicationExit", &SystemLogic::HandleApplicationExit, this);
    r.AttachHandler("TogglePlay", &SystemLogic::HandleTogglePlay, this);
  }

  void SystemLogic::GameLoad()
  {
    //if (!_isPlaying)
      //return;


#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      myComp->_pScript = AllScripts::MyConstruct(myComp->_sName.toString());
    }
#else
    // new smth
    // C# Script
#endif
  }

  void SystemLogic::GamePreInit()
  {
#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (myComp->_pScript == nullptr)
        continue;
      Entity en = G_ECMANAGER->getEntity(itr);
      myComp->_pScript->SetEntity(en);
    }
#else
    // C#
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr)
        continue;
      MyScript->_MonoData._pInstance = MonoWrapper::ConstructObject(MyScript->_ScriptName.toString());
      MyScript->_MonoData._GCHandle = MonoWrapper::ConstructGCHandle(MyScript->_MonoData._pInstance);
      int ID = G_ECMANAGER->getObjId(itrS);
      MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, "id", ID);
    }
#endif
  }

  void SystemLogic::GameInit()
  {
    if (!_isPlaying)
      return;
    _Inited = true;
#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (myComp->_pScript == nullptr || !myComp->_isActive)
        continue;
      myComp->_pScript->Init();
    }
#else
    // C# Script
    MonoBind::Bind();
    // Init base functions
    baseInit =            MonoWrapper::GetObjectMethod("Init", "UniBehaviour");
    baseLateInit =        MonoWrapper::GetObjectMethod("LateInit", "UniBehaviour");
    baseUpdate =          MonoWrapper::GetObjectMethod("Update", "UniBehaviour");
    baseExit =            MonoWrapper::GetObjectMethod("Exit", "UniBehaviour");
    baseCollisionEnter =  MonoWrapper::GetObjectMethod("OnCollisionEnter", "UniBehaviour");
    baseTriggerEnter =    MonoWrapper::GetObjectMethod("OnTriggerEnter", "UniBehaviour");

    // C# scripts init
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr || !MyScript->_isActive)
        continue;
      MonoMethod* MyInit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseInit);
      MonoWrapper::InvokeMethod(MyInit, MyScript->_MonoData._pInstance);
    }
#endif
  }

  void SystemLogic::GameLateInit()
  {
#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (myComp->_pScript == nullptr || !myComp->_isActive)
        continue;
      myComp->_pScript->LateInit();
    }
#else
    // C# scripts init
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr || !MyScript->_isActive)
        continue;
      MonoMethod* MyLateInit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseLateInit);
      MonoWrapper::InvokeMethod(MyLateInit, MyScript->_MonoData._pInstance);
    }
#endif
  }

  void SystemLogic::Update()
  {
    if (!_isPlaying)
      return;
    if (!_Inited)
    {
      GamePreInit();
      GameInit();
      GameLateInit();
    }
    ////Run Script?
#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (!myComp->_isActive)
        continue;
      myComp->_pScript->Update();
    }
#else
    // C# Scripts Update
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr || !MyScript->_isActive)
        continue;
      MonoMethod* MyUpdate = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseUpdate);
      MonoWrapper::InvokeMethod(MyUpdate, MyScript->_MonoData._pInstance);
    }
#endif
  }

  void SystemLogic::FixedUpdate()
  {
    if (!_isPlaying)
      return;
  }

  void SystemLogic::GameGameExit()
  {
#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (myComp->_isActive && myComp->_pScript)
        myComp->_pScript->Exit();
    }
#else
    // C# Scripts Exit
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr)
        continue;
      if (MyScript->_isActive)
      {
        MonoMethod* MyExit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseExit);
        MonoWrapper::InvokeMethod(MyExit, MyScript->_MonoData._pInstance);
      }
      MonoWrapper::FreeGCHandle(MyScript->_MonoData._GCHandle);
    }
#endif
  }

  void SystemLogic::GameExit()
  {
    _Loaded = false;
#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      // Delete memory
      delete myComp->_pScript;
      myComp->_pScript = nullptr;
    }
#else
#endif
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
#ifdef C_ENV
    ComponentCScript* comp1 = _obj1.getComponent<ComponentCScript>();
    ComponentCScript* comp2 = _obj2.getComponent<ComponentCScript>();
    if(comp1)
        if (comp1->_pScript)
            comp1->_pScript->OnCollisionEnter(_obj2);
    if(comp2)
        if (comp2->_pScript)
            comp2->_pScript->OnCollisionEnter(_obj1);
#else
    // C# script
    ComponentScript* cs1 = _obj1.getComponent<ComponentScript>();
    ComponentScript* cs2 = _obj2.getComponent<ComponentScript>();
    if (cs1)
    {
      MonoMethod* MyCollisionEnter = MonoWrapper::GetDerivedMethod(cs1->_MonoData._pInstance, baseCollisionEnter);
      int cs2id = _obj2.getId();
      MonoWrapper::InvokeMethodParams(MyCollisionEnter, cs1->_MonoData._pInstance, cs2id);
    }
    if (cs2)
    {
      MonoMethod* MyCollisionEnter = MonoWrapper::GetDerivedMethod(cs2->_MonoData._pInstance, baseCollisionEnter);
      int cs1id = _obj1.getId();
      MonoWrapper::InvokeMethodParams(MyCollisionEnter, cs2->_MonoData._pInstance, cs1id);
    }
#endif
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
#ifdef C_ENV
    ComponentCScript* comp1 = _obj1.getComponent<ComponentCScript>();
    ComponentCScript* comp2 = _obj2.getComponent<ComponentCScript>();
    if (comp1)
      if (comp1->_pScript)
        comp1->_pScript->OnTriggerEnter(_obj2);
    if (comp2)
      if (comp2->_pScript)
        comp2->_pScript->OnTriggerEnter(_obj1);
#else
    // C# script
    ComponentScript* cs1 = _obj1.getComponent<ComponentScript>();
    ComponentScript* cs2 = _obj2.getComponent<ComponentScript>();
    if (cs1)
    {
      MonoMethod* MyTriggerEnter = MonoWrapper::GetDerivedMethod(cs1->_MonoData._pInstance, baseTriggerEnter);
      int cs2id = _obj2.getId();
      MonoWrapper::InvokeMethodParams(MyTriggerEnter, cs1->_MonoData._pInstance, cs2id);
    }
    if (cs2)
    {
      MonoMethod* MyTriggerEnter = MonoWrapper::GetDerivedMethod(cs2->_MonoData._pInstance, baseTriggerEnter);
      int cs1id = _obj1.getId();
      MonoWrapper::InvokeMethodParams(MyTriggerEnter, cs2->_MonoData._pInstance, cs1id);
    }
#endif
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

  void SystemLogic::HandleMsg(MessageScriptRequest& msg)
  {
    std::cout << "Received Request for script!" << std::endl;
#ifdef C_ENV
    auto itr = G_ECMANAGER->begin<CScriptComponent>();
    auto eitr = G_ECMANAGER->end<CScriptComponent>();
    for (; itr != eitr; ++itr)
    {
      CScriptComponent* comp = G_ECMANAGER->getComponent<CScriptComponent>(itr);
      if (comp == nullptr || comp->_iTag != msg.tagID)
        continue;
      // Found script instance
      msg._pScript = comp->_pScript;
      // Break out of loop
      break;
    }
#else

#endif
  }

  void SystemLogic::HandleApplicationExit(MessageApplicationExit& msg)
  {
    // Handle msg function here
    std::cout << "Hello from SystemLogic!" << std::endl;
  }

  void SystemLogic::HandleTogglePlay(MessageTogglePlay& msg)
  {
    // Handle msg here.
    std::cout << "Hello from SystemLogic!" << std::endl;
    std::cout << "TogglePlay value: " << msg.isPlaying << std::endl;

    _isPlaying = msg.isPlaying;
    if (!_isPlaying)
    {
      GameGameExit();
      _Inited = false;
    }
    //if (!_isPlaying)
    //  GameExit();
  }
}
