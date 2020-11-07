#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"
// Testing
#include <iostream>

class PossessScript : public IScript
{
  Entity PlayerEntity;
public:

  virtual void Init() override;

  virtual void Update() override;

  virtual void Exit() override;

  virtual void OnCollisionEnter(Entity other) override;

};