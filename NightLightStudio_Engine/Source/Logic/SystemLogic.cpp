#include "SystemLogic.h"
#include "..\Mono\MonoWrapper.h" // Mono functions wrapped

namespace NS_LOGIC
{

  void SystemLogic::Load()
  {
    MonoWrapper::InitMono();
  }

  void SystemLogic::Init()
  {
    //Start up Scripting system??
  }

  void SystemLogic::FixedUpdate()
  {

  }

  void SystemLogic::Update()
  {
    //Run Script?
  }

  void SystemLogic::Free()
  {
  }

  void SystemLogic::Exit()
  {
    DestroyInstance();
  }
}
