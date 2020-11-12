#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"

// Components
#include "../../Component/ComponentCollider.h"
#include "../../Component/ComponentCScript.h"

class CauldronScript : public IScript
{
  const float regenAmount = 1.0f;
public:
  std::string playerName = "Player";
  Entity PlayerENT;
  ComponentCollider* PlayerCOL;
  ComponentCScript* PlayerCSCRIPT;

  virtual void Init() override;

  virtual void Update() override;

  virtual void Exit() override;

  virtual void OnCollisionEnter(Entity other) override;

  virtual void OnTriggerEnter(Entity other) override;
};