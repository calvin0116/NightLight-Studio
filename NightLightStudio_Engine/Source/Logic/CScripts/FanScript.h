#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"
// Testing
#include <iostream>
#include <string>
#include "../../Component/ComponentCScript.h"
#include "../../Component/ComponentTransform.h"

class FanScript : public IScript
{
  const float wind_up_time = 1.0f;
public:
  // Exposed variables
  Entity MyPlayerEntity;
  //int MyPlayerID;
  //Entity talisman1;
  Entity obb1;
  bool Activate = false;
  float timePassed = 0.0f;
  ComponentCScript* playerScriptComp = nullptr;
  ComponentCScript* obb1ScriptComp = nullptr;
    
  std::string playerName = "Player";
  std::string obbName = "OBB_1";

  virtual void Init() override;

  virtual void Update() override;

  virtual void Exit() override;

  virtual void OnCollisionEnter(Entity other) override;

  virtual void OnTriggerEnter(Entity other) override;
};