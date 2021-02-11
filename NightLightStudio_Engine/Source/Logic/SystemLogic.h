#pragma once
#include "..\Core\MySystem.h"
#include <vector>
#include "../Component/Components.h" // G_ECMANAGER, Entity type

// For messaging/event
#include "../Messaging/SystemReceiver.h"
#include "../Messaging/Messages/MessageScriptRequest.h"
#include "../Messaging/Messages/MessageApplicationExit.h"
#include "../Messaging/Messages/MessageTogglePlay.h"
// Saving of reloaded scripts' values
#include <any>

//#define C_ENV
#define CS_ENV

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
    MonoMethod* baseInit              = nullptr;
    MonoMethod* baseLateInit          = nullptr;
    MonoMethod* baseUpdate            = nullptr;
    MonoMethod* baseFixedUpdate       = nullptr;
    MonoMethod* baseExit              = nullptr;
    MonoMethod* baseCollisionEnter    = nullptr;
    MonoMethod* baseTriggerEnter      = nullptr;
    MonoMethod* baseCollisionStay     = nullptr;
    MonoMethod* baseTriggerStay       = nullptr;
    MonoMethod* baseCollisionExit     = nullptr;
    MonoMethod* baseTriggerExit       = nullptr;

    // For C# Inspection
    std::unordered_map<std::string, std::any> ScriptValues;
    
    // For receiving event/message
    SystemMessaging::SystemReceiver r;
    // For script saving/loading
    void SaveValues();
    void LoadValues();
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
    void FixedUpdate(float dt) override;

    void Free() override;
    void Exit() override;

    void OnCollisionEnter(Entity _obj1, Entity _obj2);
    void OnCollisionStay(Entity _obj1, Entity _obj2);
    void OnCollisionExit(Entity _obj1, Entity _obj2);
    void OnTriggerEnter(Entity _obj1, Entity _obj2);
    void OnTriggerStay(Entity _obj1, Entity _obj2);
    void OnTriggerExit(Entity _obj1, Entity _obj2);

#ifdef C_ENV
    void HandleMsg(MessageScriptRequest&);
#endif
    //void HandleApplicationExit(MessageApplicationExit&);
    void HandleTogglePlay(MessageTogglePlay&);
  };

  static SystemLogic* SYS_LOGIC = SystemLogic::GetInstance();
}