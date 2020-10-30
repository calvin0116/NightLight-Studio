#include "SystemLogic.h"
#include "..\Mono\MonoWrapper.h" // Mono functions wrapped
// #include "..\Mono\Binding\Binder.h" // Bind C++ to C# functions

#include "../Input/SystemInput.h" // For testing

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
  }

  void SystemLogic::GameInit()
  {
    // MonoBind::Bind();
    // Init CS
    // MonoMethod* m_Init = MonoWrapper::GetObjectMethod("Init", "UniBehaviour");
  }

  void SystemLogic::Update()
  {
    //Run Script?
  }

  void SystemLogic::FixedUpdate()
  {

  }

  void SystemLogic::GameExit()
  {

  }

  void SystemLogic::Free()
  {
  }

  void SystemLogic::Exit()
  {
    DestroyInstance();
  }
}
