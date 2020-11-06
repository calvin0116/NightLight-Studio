#pragma once
#include "..\Core\MySystem.h"
#include <vector>
#include "../Component/Components.h" // G_ECMANAGER, Entity type

namespace NS_LOGIC
{
  class SystemLogic : public MySystem, public Singleton<SystemLogic>
  {
    friend Singleton<SystemLogic>;

    // Temporary C++ scripting
    // Scripts stored in script component
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

    void OnCollisionEnter(Entity _obj1, Entity _obj2);
    void OnCollisionStay(Entity _obj1, Entity _obj2);
    void OnCollisionExit(Entity _obj1, Entity _obj2);
    void OnTriggerEnter(Entity _obj1, Entity _obj2);
    void OnTriggerStay(Entity _obj1, Entity _obj2);
    void OnTriggerExit(Entity _obj1, Entity _obj2);
  };

  static SystemLogic* SYS_LOGIC = SystemLogic::GetInstance();
}