#include "SystemLogic.h"
#include "..\Mono\MonoWrapper.h" // Mono functions wrapped
#include "..\Mono\Binding\Binder.h" // Bind C++ to C# functions

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
          if (_isPlaying)
          {
            GameExit();
          }
          _isPlaying = !_isPlaying;
          std::cout << "Logic Playing: " << _isPlaying << std::endl;
        }
      });

    // Attach handler
    r.AttachHandler("ScriptRequest", &SystemLogic::HandleMsg, this);
    r.AttachHandler("ApplicationExit", &SystemLogic::HandleApplicationExit, this);
    r.AttachHandler("TogglePlay", &SystemLogic::HandleTogglePlay, this);
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
      if(myComp->_pScript) // Check if script constructed
        myComp->_pScript->SetEntity(G_ECMANAGER->getEntity(itr));
    }
    // new smth
    // C# Script
  }

  void SystemLogic::GameInit()
  {
    if (!_isPlaying)
      return;
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (!myComp->_isActive)
        continue;
      myComp->_pScript->Init();
    }

    // C# Script
    MonoBind::Bind();
    // Init base functions
    baseInit =            MonoWrapper::GetObjectMethod("Init", "UniBehaviour");
    baseUpdate =          MonoWrapper::GetObjectMethod("Update", "UniBehaviour");
    baseExit =            MonoWrapper::GetObjectMethod("Exit", "UniBehaviour");
    baseCollisionEnter =  MonoWrapper::GetObjectMethod("OnCollisionEnter", "UniBehaviour");

    // C# scripts init
 /*   auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr || !MyScript->_isActive)
        continue;
      MyScript->_MonoData._pInstance = MonoWrapper::ConstructObject(MyScript->_ScriptName.toString());
      MyScript->_MonoData._GCHandle = MonoWrapper::ConstructGCHandle(MyScript->_MonoData._pInstance);
      int ID = G_ECMANAGER->getObjId(itrS);
      MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, "id", ID);
      MonoMethod* MyInit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseInit);
      MonoWrapper::InvokeMethod(MyInit, MyScript->_MonoData._pInstance);
    }*/
    _Inited = true;
  }

  void SystemLogic::Update()
  {
    if (!_isPlaying)
      return;
    if (!_Loaded)
    {
      GameLoad();
      _Loaded = true;
    }
    if (!_Inited)
    {
      GameInit();
      _Inited = true;
    }
    ////Run Script?
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (!myComp->_isActive)
        continue;
      myComp->_pScript->Update();
    }

    //// C# Scripts Update
    //auto itrS = G_ECMANAGER->begin<ComponentScript>();
    //auto itrE = G_ECMANAGER->end<ComponentScript>();
    //for (; itrS != itrE; ++itrS)
    //{
    //  ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
    //  if (MyScript == nullptr || !MyScript->_isActive)
    //    continue;
    //  MonoMethod* MyUpdate = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseUpdate);
    //  MonoWrapper::InvokeMethod(MyUpdate, MyScript->_MonoData._pInstance);
    //}
  }

  void SystemLogic::FixedUpdate()
  {
    if (!_isPlaying)
      return;
  }

  void SystemLogic::GameExit()
  {
    _Inited = false;
    _Loaded = false;
    auto itr = G_ECMANAGER->begin<ComponentCScript>();
    auto itrEnd = G_ECMANAGER->end<ComponentCScript>();
    for (; itr != itrEnd; ++itr)
    {
      ComponentCScript* myComp = G_ECMANAGER->getComponent<ComponentCScript>(itr);
      if (myComp->_isActive)
        myComp->_pScript->Exit();

      // Delete memory
      delete myComp->_pScript;
      myComp->_pScript = nullptr;
    }

    //// C# Scripts Exit
    //auto itrS = G_ECMANAGER->begin<ComponentScript>();
    //auto itrE = G_ECMANAGER->end<ComponentScript>();
    //for (; itrS != itrE; ++itrS)
    //{
    //  ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
    //  if (MyScript == nullptr || !MyScript->_isActive)
    //    continue;
    //  MonoWrapper::FreeGCHandle(MyScript->_MonoData._GCHandle);
    //  MonoMethod* MyExit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseExit);
    //  MonoWrapper::InvokeMethod(MyExit, MyScript->_MonoData._pInstance);
    //}
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
    ComponentCScript* comp1 = _obj1.getComponent<ComponentCScript>();
    ComponentCScript* comp2 = _obj2.getComponent<ComponentCScript>();
    if(comp1)
        if (comp1->_pScript)
            comp1->_pScript->OnCollisionEnter(_obj2);
    if(comp2)
        if (comp2->_pScript)
            comp2->_pScript->OnCollisionEnter(_obj1);

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

  void SystemLogic::HandleMsg(MessageScriptRequest& msg)
  {
    std::cout << "Received Request for script!" << std::endl;
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
    //_isPlaying = msg.isPlaying;
    //if (!_isPlaying)
    //  GameExit();
  }
}
