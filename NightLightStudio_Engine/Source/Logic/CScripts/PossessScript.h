#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"
// Testing
#include <iostream>
#include "..\..\Component\ComponentLight.h"

class PossessScript : public IScript
{
  Entity PlayerEntity;
  ComponentLight* CompLight;
public:
  // Exposed variables
  const bool Possessable = true;
  bool Possessed = false;
  float LightIntensity = 5.0f;
  const float MinIntensity = 5.0f;
  const float MaxIntensity = 100.0f;

  virtual void Init() override;

  virtual void Update() override;

  virtual void Exit() override;

  virtual void OnCollisionEnter(Entity other) override;

  virtual void ChangeLight(float val);
};