#pragma once
#include "..\Core\MySystem.h"

namespace NS_LOGIC
{
  class SystemLogic : public MySystem, public Singleton<SystemLogic>
  {
    friend Singleton<SystemLogic>;
  public:
    // System functions
    void Load() override;
    // void EarlyInit() override {};
    void Init() override;
    //void LateInit() override {};

    void FixedUpdate() override;
    void Update() override;

    void Free();
    void Exit() override;
  };

  static SystemLogic* SYS_LOGIC = SystemLogic::GetInstance();
}