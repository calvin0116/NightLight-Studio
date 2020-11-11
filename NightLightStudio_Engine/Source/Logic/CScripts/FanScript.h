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
  Entity obb1;
  Entity CameraEntity;
  ComponentCScript* playerScriptComp = nullptr;
  ComponentCScript* obb1ScriptComp = nullptr;
  ComponentCScript* cameraScriptComp = nullptr;

  bool Activate = false;
  float timePassed = 0.0f;

    
  std::string playerName = "Player";
  std::string obbName = "OBB_1";
  std::string camName = "PlayerCamera";

  virtual void Init() override;

  virtual void Update() override;

  virtual void Exit() override;

  virtual void OnCollisionEnter(Entity other) override;

  virtual void OnTriggerEnter(Entity other) override;
};