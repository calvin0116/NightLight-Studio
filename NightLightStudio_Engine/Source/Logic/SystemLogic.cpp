#include "SystemLogic.h"
#include "..\Mono\MonoWrapper.h" // Mono functions wrapped
#include "..\Mono\Binding\Binder.h" // Bind C++ to C# functions

#include "../Input/SystemInput.h" // For testing
//#include "CScripts/AllScripts.h"

// Scene change
#include "../Core/SceneManager.h"

// Tracy
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"
// For editor console cout
#include "../Editor/SystemEditor.h"

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

  void SystemLogic::SaveValues()
  {
    ScriptValues.clear();
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr)
        continue;
      ComponentScript::data& MonoData = MyScript->_MonoData;
      if (MonoData._pInstance)
      {
        MonoClass* klass = MonoWrapper::GetMonoClass(MonoData._pInstance);
        void* iter = NULL;
        MonoClassField* field = mono_class_get_fields(klass, &iter);
        while (field)
        {
          const char* var_name = mono_field_get_name(field);
          MonoType* type = mono_field_get_type(field);
          int var_typeid = mono_type_get_type(type);
          unsigned var_flag = mono_field_get_flags(field);
          // Store values into local vector
          if (var_flag == MONO_FIELD_ATTR_PUBLIC)
          {
            std::string tempVar(var_name);
            tempVar += MonoWrapper::GetObjectFieldValue<int>(MonoData._pInstance, "id");
            switch (var_typeid)
            {
            case MONO_TYPE_BOOLEAN:
              tempVar += "bool";
              ScriptValues.emplace(tempVar, MonoWrapper::GetObjectFieldValue<bool>(MonoData._pInstance, var_name));
              break;
            case MONO_TYPE_I4:
              tempVar += "int";
              ScriptValues.emplace(tempVar, MonoWrapper::GetObjectFieldValue<int>(MonoData._pInstance, var_name));
              break;
            case MONO_TYPE_U4:
              tempVar += "unsigned";
              ScriptValues.emplace(tempVar, MonoWrapper::GetObjectFieldValue<unsigned>(MonoData._pInstance, var_name));
              break;
            case MONO_TYPE_R4:
              tempVar += "float";
              ScriptValues.emplace(tempVar, MonoWrapper::GetObjectFieldValue<float>(MonoData._pInstance, var_name));
              break;
            case MONO_TYPE_R8:
              tempVar += "double";
              ScriptValues.emplace(tempVar, MonoWrapper::GetObjectFieldValue<double>(MonoData._pInstance, var_name));
              break;
            case MONO_TYPE_STRING:
              tempVar += "string";
              if (MonoWrapper::GetObjectFieldValue<MonoString*>(MonoData._pInstance, var_name))
                ScriptValues.emplace(tempVar, MonoWrapper::ToString(MonoWrapper::GetObjectFieldValue<MonoString*>(MonoData._pInstance, var_name)));
              else
                ScriptValues.emplace(tempVar, std::string(""));
              break;
            case MONO_TYPE_SZARRAY:
            {
              MonoArrayType* array_type = mono_type_get_array_type(type);              
              //ED_OUT("My Array Type: " + std::to_string(var_typeid));
              MonoClass* raw_class_type = array_type->eklass;
              MonoType* mono_type = mono_class_get_type(raw_class_type);

              //MonoArray* mono_array = MonoWrapper::GetObjectFieldValue<MonoArray*>(MonoData._pInstance, var_name);
              int my_type = mono_type_get_type(mono_type);
              //int arr_sz = mono_array_length(mono_array);
              ED_OUT("My MONO Type: " + std::to_string(my_type));
              //ED_OUT("My Arr Size: " + std::to_string(arr_sz));
              //switch()
              break;
            }
            default:
              // Unhandled type, do nothing.
              ED_OUT("My MONO Type: " + std::to_string(var_typeid));
              break;
            }
          }
          // Move to next field
          field = mono_class_get_fields(klass, &iter);
        }
      }
    }
  }

  void SystemLogic::LoadValues()
  {
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr)
        continue;
      MonoClass* klass = MonoWrapper::GetMonoClass(MyScript->_MonoData._pInstance);
      void* iter = NULL;
      MonoClassField* field = mono_class_get_fields(klass, &iter);
      while (field)
      {
        const char* var_name = mono_field_get_name(field);
        int var_typeid = mono_type_get_type(mono_field_get_type(field));
        unsigned var_flag = mono_field_get_flags(field);
        // Store values into local vector
        if (var_flag == MONO_FIELD_ATTR_PUBLIC)
        {
          std::string tempVar(var_name);
          tempVar += MyScript->objId;
            if (var_typeid == MONO_TYPE_BOOLEAN)
            {
              tempVar += "bool";
              if (ScriptValues.find(tempVar) != ScriptValues.end())
              {
                bool val = std::any_cast<bool>(ScriptValues[tempVar]);
                MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, var_name, val);
              }
            }
            else if (var_typeid == MONO_TYPE_I4)
            {
              tempVar += "int";
              if (ScriptValues.find(tempVar) != ScriptValues.end())
              {
                int val = std::any_cast<int>(ScriptValues[tempVar]);
                MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, var_name, val);
              }
            }
            else if (var_typeid == MONO_TYPE_U4)
            {
              tempVar += "unsigned";
              if (ScriptValues.find(tempVar) != ScriptValues.end())
              {
                unsigned val = std::any_cast<unsigned>(ScriptValues[tempVar]);
                MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, var_name, val);
              }
            }
            else if (var_typeid == MONO_TYPE_R4)
            {
              tempVar += "float";
              if (ScriptValues.find(tempVar) != ScriptValues.end())
              {
                float val = std::any_cast<float>(ScriptValues[tempVar]);
                MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, var_name, val);
              }
            }
            else if (var_typeid == MONO_TYPE_R8)
            {
              tempVar += "double";
              if (ScriptValues.find(tempVar) != ScriptValues.end())
              {
                double val = std::any_cast<double>(ScriptValues[tempVar]);
                MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, var_name, val);
              }
            }
            else if (var_typeid == MONO_TYPE_STRING)
            {
              tempVar += "string";
              if (ScriptValues.find(tempVar) != ScriptValues.end())
              {
                std::string val = std::any_cast<std::string>(ScriptValues[tempVar]);
                if (!val.empty())
                  MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, var_name, *MonoWrapper::ToMonoString(val));
              }
            }
        }
        // Move to next field
        field = mono_class_get_fields(klass, &iter);
      }
    }
  }

  void SystemLogic::Load()
  {
    MonoWrapper::InitMono();
#ifdef _EDITOR
    MonoWrapper::UnloadScriptDomain();
    MonoWrapper::CompileInitScripts();
#endif
    MonoWrapper::LoadScriptDomain();
    MonoWrapper::OpenDLL();
    //MonoWrapper::ReloadScripts();
  }

  void SystemLogic::Init()
  {
#ifdef _EDITOR
    //Start up Scripting system??
    SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ReloadScripts", SystemInput_ns::IKEY_END, "ScriptReload", SystemInput_ns::OnRelease, [this]()
      {
        //Only if mouse wheel + alt button is pressed, camera will move.
        if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IKEY_END))
        {
          if (!_isPlaying)
          {
            SaveValues();
            MonoWrapper::ReloadScripts();
            auto itrS = G_ECMANAGER->begin<ComponentScript>();
            auto itrE = G_ECMANAGER->end<ComponentScript>();
            //auto itrE = G_ECMANAGER->end<ComponentScript>();
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
            LoadValues();
          }
        }
      });
#endif
    // Attach handler
#ifdef C_ENV
    r.AttachHandler("ScriptRequest", &SystemLogic::HandleMsg, this);
#endif // C_ENV
    //r.AttachHandler("ApplicationExit", &SystemLogic::HandleApplicationExit, this);
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
#endif
#ifdef CS_ENV
    // new smth
    // C# Script
#endif
  }

  void SystemLogic::GameInit()
  {
    // Construct scripts and get values from serialising
    //auto itrS = G_ECMANAGER->begin<ComponentScript>();
    //auto itrE = G_ECMANAGER->end<ComponentScript>();
    //for (; itrS != itrE; ++itrS)
    //{
    //  std::cout << "Constructed!" << std::endl;
    //  ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
    //  if (MyScript == nullptr)
    //    continue;
    //  MyScript->_MonoData._pInstance = MonoWrapper::ConstructObject(MyScript->_ScriptName.toString());
    //  MyScript->_MonoData._GCHandle = MonoWrapper::ConstructGCHandle(MyScript->_MonoData._pInstance);
    //  int ID = G_ECMANAGER->getObjId(itrS);
    //  MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, "id", ID);
    //}
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
#endif
#ifdef CS_ENV
    // C#
    // Reload Scripts
    //std::cout << "GamePreInit" << std::endl;
    //SaveValues();
#ifdef _EDITOR
    //MonoWrapper::ReloadScripts();
#endif
    // Create script instance
    //auto itrS = G_ECMANAGER->begin<ComponentScript>();
    //auto itrE = G_ECMANAGER->end<ComponentScript>();
    //for (; itrS != itrE; ++itrS)
    //{
    //  ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
    //  if (MyScript == nullptr)
    //    continue;
    //  MyScript->_MonoData._pInstance = MonoWrapper::ConstructObject(MyScript->_ScriptName.toString());
    //  MyScript->_MonoData._GCHandle = MonoWrapper::ConstructGCHandle(MyScript->_MonoData._pInstance);
    //  int ID = G_ECMANAGER->getObjId(itrS);
    //  MonoWrapper::SetObjectFieldValue(MyScript->_MonoData._pInstance, "id", ID);
    //}
#endif
  }

  void SystemLogic::GameGameInit()
  {
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
#endif
#ifdef CS_ENV
    // C# Script
    //std::cout << "GameGameInit" << std::endl;
    MonoBind::Bind();
    // Init base functions
    baseInit = MonoWrapper::GetObjectMethod("Init", "UniBehaviour");
    baseLateInit = MonoWrapper::GetObjectMethod("LateInit", "UniBehaviour");
    baseUpdate = MonoWrapper::GetObjectMethod("Update", "UniBehaviour");
    baseFixedUpdate = MonoWrapper::GetObjectMethod("FixedUpdate", "UniBehaviour");
    baseExit = MonoWrapper::GetObjectMethod("Exit", "UniBehaviour");
    baseCollisionEnter = MonoWrapper::GetObjectMethod("OnCollisionEnter", "UniBehaviour");
    baseCollisionStay = MonoWrapper::GetObjectMethod("OnCollisionStay", "UniBehaviour");
    baseCollisionExit = MonoWrapper::GetObjectMethod("OnCollisionExit", "UniBehaviour");
    baseTriggerEnter = MonoWrapper::GetObjectMethod("OnTriggerEnter", "UniBehaviour");
    baseTriggerStay = MonoWrapper::GetObjectMethod("OnTriggerStay", "UniBehaviour");
    baseTriggerExit = MonoWrapper::GetObjectMethod("OnTriggerExit", "UniBehaviour");

    // C# scripts init
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr || !MyScript->_isActive)
        continue;

      // Debugging purposes
      std::string output = "SystemLogic::GameInit calling " + std::to_string(MyScript->objId) + "'s script: " + std::string(MyScript->_ScriptName) + "...";
      TracyMessage(output.c_str(), output.size());
      SPEEDLOG(output);

      MonoMethod* MyInit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseInit);
      MonoWrapper::InvokeMethod(MyInit, MyScript->_MonoData._pInstance);

      // Debugging purposes
      output = "...Done.";
      TracyMessage(output.c_str(), output.size());
      SPEEDLOG(output);
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
#endif
#ifdef CS_ENV
    // C# scripts init
    //std::cout << "GameLateInit" << std::endl;
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr || !MyScript->_isActive)
        continue;

      // Debugging purposes
      std::string output = "SystemLogic::GameLateInit calling " + std::to_string(MyScript->objId) + "'s script: " + std::string(MyScript->_ScriptName) + "...";
      TracyMessage(output.c_str(), output.size());
      SPEEDLOG(output);

      MonoMethod* MyLateInit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseLateInit);
      MonoWrapper::InvokeMethod(MyLateInit, MyScript->_MonoData._pInstance);

      // Debugging purposes
      output = "...Done.";
      TracyMessage(output.c_str(), output.size());
      SPEEDLOG(output);
    }
#endif
  }

  void SystemLogic::Update()
  {
    // Tracy
    // Zone Color: Light Pink
    ZoneScopedNC("Logic: Update", 0xfaa5e2);

    if (!_isPlaying)
      return;
    if (!_Inited)
    {
      _Inited = true;
      GamePreInit();
      GameGameInit();
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
#endif
#ifdef CS_ENV
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

  void SystemLogic::FixedUpdate(float dt)
  {
      // Tracy
      // Zone Color: Pink
      ZoneScopedNC("Logic: Fixed Update", 0xff24c1);

      dt;
    if (!_isPlaying || !_Inited)
      return;
#ifdef CS_ENV
    // C# Scripts Update
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      if (MyScript == nullptr || !MyScript->_isActive)
        continue;
      MonoMethod* MyFixedUpdate = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseFixedUpdate);
      MonoWrapper::InvokeMethod(MyFixedUpdate, MyScript->_MonoData._pInstance);
    }
#endif
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
#endif
#ifdef CS_ENV
    // C# Scripts Exit
    //std::cout << "GameGameExit" << std::endl;
    auto itrS = G_ECMANAGER->begin<ComponentScript>();
    auto itrE = G_ECMANAGER->end<ComponentScript>();
    for (; itrS != itrE; ++itrS)
    {
      ComponentScript* MyScript = G_ECMANAGER->getComponent<ComponentScript>(itrS);
      //ComponentTransform* trans = G_ECMANAGER->getComponent<ComponentTransform>(itrS);
      if (MyScript == nullptr)
        continue;
      if (MyScript->_isActive)
      {
        // Debugging purposes
        std::string output = "SystemLogic::GameExit calling " + std::to_string(MyScript->objId) + "'s script: " + std::string(MyScript->_ScriptName) + "...";
        TracyMessage(output.c_str(), output.size());
        SPEEDLOG(output);

        MonoMethod* MyExit = MonoWrapper::GetDerivedMethod(MyScript->_MonoData._pInstance, baseExit);
        MonoWrapper::InvokeMethod(MyExit, MyScript->_MonoData._pInstance);

        // Debugging purposes
        output = "...Done.";
        TracyMessage(output.c_str(), output.size());
        SPEEDLOG(output);
      }
      //MonoWrapper::FreeGCHandle(MyScript->_MonoData._GCHandle);
    }
    //LoadValues();
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
#endif
#ifdef CS_ENV
    if (_isPlaying) // Scene changed
    {
      GameGameExit();
      _Inited = false;
    }
#endif
  }

  void SystemLogic::Free()
  {
  }

  void SystemLogic::Exit()
  {
    MonoWrapper::MonoExit();
    DestroyInstance();
  }

  void SystemLogic::OnCollisionEnter(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying || !_Inited)
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
#endif
#ifdef CS_ENV
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
    if (!_isPlaying || !_Inited)
      return;
#ifdef CS_ENV
    // C# script
    ComponentScript* cs1 = _obj1.getComponent<ComponentScript>();
    ComponentScript* cs2 = _obj2.getComponent<ComponentScript>();
    if (cs1)
    {
      MonoMethod* MyCollisionStay = MonoWrapper::GetDerivedMethod(cs1->_MonoData._pInstance, baseCollisionStay);
      int cs2id = _obj2.getId();
      MonoWrapper::InvokeMethodParams(MyCollisionStay, cs1->_MonoData._pInstance, cs2id);
    }
    if (cs2)
    {
      MonoMethod* MyCollisionStay = MonoWrapper::GetDerivedMethod(cs2->_MonoData._pInstance, baseCollisionStay);
      int cs1id = _obj1.getId();
      MonoWrapper::InvokeMethodParams(MyCollisionStay, cs2->_MonoData._pInstance, cs1id);
    }
#endif
  }

  void SystemLogic::OnCollisionExit(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying || !_Inited)
      return; 
#ifdef CS_ENV
      // C# script
      ComponentScript* cs1 = _obj1.getComponent<ComponentScript>();
    ComponentScript* cs2 = _obj2.getComponent<ComponentScript>();
    if (cs1)
    {
      MonoMethod* MyCollisionExit = MonoWrapper::GetDerivedMethod(cs1->_MonoData._pInstance, baseCollisionExit);
      int cs2id = _obj2.getId();
      MonoWrapper::InvokeMethodParams(MyCollisionExit, cs1->_MonoData._pInstance, cs2id);
    }
    if (cs2)
    {
      MonoMethod* MyCollisionExit = MonoWrapper::GetDerivedMethod(cs2->_MonoData._pInstance, baseCollisionExit);
      int cs1id = _obj1.getId();
      MonoWrapper::InvokeMethodParams(MyCollisionExit, cs2->_MonoData._pInstance, cs1id);
    }
#endif
  }

  void SystemLogic::OnTriggerEnter(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying || !_Inited)
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
#endif
#ifdef CS_ENV
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
    if (!_isPlaying || !_Inited)
      return;
#ifdef CS_ENV
    // C# script
    ComponentScript* cs1 = _obj1.getComponent<ComponentScript>();
    ComponentScript* cs2 = _obj2.getComponent<ComponentScript>();
    if (cs1)
    {
      MonoMethod* MyTriggerStay = MonoWrapper::GetDerivedMethod(cs1->_MonoData._pInstance, baseTriggerStay);
      int cs2id = _obj2.getId();
      MonoWrapper::InvokeMethodParams(MyTriggerStay, cs1->_MonoData._pInstance, cs2id);
    }
    if (cs2)
    {
      MonoMethod* MyTriggerStay = MonoWrapper::GetDerivedMethod(cs2->_MonoData._pInstance, baseTriggerStay);
      int cs1id = _obj1.getId();
      MonoWrapper::InvokeMethodParams(MyTriggerStay, cs2->_MonoData._pInstance, cs1id);
    }
#endif
  }

  void SystemLogic::OnTriggerExit(Entity _obj1, Entity _obj2)
  {
    if (!_isPlaying || !_Inited)
      return;
#ifdef CS_ENV
    // C# script
    ComponentScript* cs1 = _obj1.getComponent<ComponentScript>();
    ComponentScript* cs2 = _obj2.getComponent<ComponentScript>();
    if (cs1)
    {
      MonoMethod* MyTriggerExit = MonoWrapper::GetDerivedMethod(cs1->_MonoData._pInstance, baseTriggerExit);
      int cs2id = _obj2.getId();
      MonoWrapper::InvokeMethodParams(MyTriggerExit, cs1->_MonoData._pInstance, cs2id);
    }
    if (cs2)
    {
      MonoMethod* MyTriggerExit = MonoWrapper::GetDerivedMethod(cs2->_MonoData._pInstance, baseTriggerExit);
      int cs1id = _obj1.getId();
      MonoWrapper::InvokeMethodParams(MyTriggerExit, cs2->_MonoData._pInstance, cs1id);
    }
#endif
  }

#ifdef C_ENV
  void SystemLogic::HandleMsg(MessageScriptRequest& msg)
  {
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
#endif

  //void SystemLogic::HandleApplicationExit(MessageApplicationExit& msg)
  //{
  //  // Handle msg function here
  //  
  //}

  void SystemLogic::HandleTogglePlay(MessageTogglePlay& msg)
  {
    // Handle msg here.
    if (msg.GetID() != "TogglePlay")
      return;
    //TracyMessageL("SystemLogic::HandleTogglePlay: Toggle Play");
    //SPEEDLOG("SystemLogic::HandleTogglePlay: Toggle Play");
    //std::cout << "Toggle Play" << std::endl;
    _isPlaying = msg.isPlaying;
    if (!_isPlaying && _Inited)
    {
      GameGameExit();
      _Inited = false;

      // Reset values
    }
  }
}
