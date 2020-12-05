#pragma once
#include "..\Core\MySystem.h"
#include <vector>
#include "../Component/Components.h" // G_ECMANAGER, Entity type

// For messaging/event
#include "../Messaging/SystemReceiver.h"
#include "../Messaging/Messages/MessageScriptRequest.h"
#include "../Messaging/Messages/MessageApplicationExit.h"
#include "../Messaging/Messages/MessageTogglePlay.h"

namespace NS_LOGIC
{
  class SystemLogic : public MySystem, public Singleton<SystemLogic>
  {
    friend Singleton<SystemLogic>;

    // Temporary C++ scripting
    // Scripts stored in script component
    static bool _isPlaying;
    static bool _Loaded;
    static bool _Inited;

    // mono base methods
    MonoMethod* baseInit;
    MonoMethod* baseLateInit;
    MonoMethod* baseUpdate;
    MonoMethod* baseFixedUpdate;
    MonoMethod* baseExit;
    MonoMethod* baseCollisionEnter;
    MonoMethod* baseTriggerEnter;
    MonoMethod* baseCollisionStay;
    MonoMethod* baseTriggerStay;
    MonoMethod* baseCollisionExit;
    MonoMethod* baseTriggerExit;
    
    // For receiving event/message
    SystemMessaging::SystemReceiver r;
  public:
    // System functions
    void Load() override;
    // void EarlyInit() override {};
    void Init() override;
    //void LateInit() override {};

    void GameLoad() override;
    void GameInit() override;
    void GameExit() override;

    void GameGameExit();
    void GamePreInit();
    void GameGameInit();
    void GameLateInit();

    void Update() override;
    void FixedUpdate() override;

    void Free() override;
    void Exit() override;

    void OnCollisionEnter(Entity _obj1, Entity _obj2);
    void OnCollisionStay(Entity _obj1, Entity _obj2);
    void OnCollisionExit(Entity _obj1, Entity _obj2);
    void OnTriggerEnter(Entity _obj1, Entity _obj2);
    void OnTriggerStay(Entity _obj1, Entity _obj2);
    void OnTriggerExit(Entity _obj1, Entity _obj2);

    void HandleMsg(MessageScriptRequest&);
    void HandleApplicationExit(MessageApplicationExit&);
    void HandleTogglePlay(MessageTogglePlay&);
  };

  static SystemLogic* SYS_LOGIC = SystemLogic::GetInstance();
}