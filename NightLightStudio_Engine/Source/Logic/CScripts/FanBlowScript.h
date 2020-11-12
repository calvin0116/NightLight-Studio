#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"

#include "../../Math/Vector.h"
#include <string>

class FanBlowScript : public IScript
{
  Entity Talisman1;
  //int Talisman1ID;
public:
  bool Activate = false;
  NlMath::Vector3D Direction = {0.0f, 0.0f, -1.0f};
  float Magnitude = 3000.0f;
  std::string talismanName;

  virtual void Init() override;

  virtual void LateInit() override;

  virtual void Update() override;

  virtual void Exit() override;

  virtual void OnCollisionEnter(Entity other) override;

  virtual void OnTriggerEnter(Entity other) override;
};