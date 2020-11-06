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

    void GameLoad() override;
    void GameInit() override;

    void Update() override;
    void FixedUpdate() override;

    void GameExit() override;

    void Free() override;
    void Exit() override;

    void OnCollisionEnter();
    void OnCollisionStay();
    void OnCollisionExit();
    void OnTriggerEnter();
    void OnTriggerStay();
    void OnTriggerExit();
  };

  static SystemLogic* SYS_LOGIC = SystemLogic::GetInstance();
}