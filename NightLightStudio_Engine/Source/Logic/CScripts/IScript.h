#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "..\..\Component\ComponentManager.h" // E_CMANAGER

class IScript
{
protected:
  Entity  MyID; // Dont touch
public:
  //IScript() = 0;
  virtual ~IScript() {}
  virtual void SetEntity(Entity _id) { MyID = _id; }
  virtual void Init() {};
  virtual void LateInit() {};
  virtual void Update() {};
  virtual void Exit() {};

  virtual void OnCollisionEnter(Entity) {};
  virtual void OnTriggerEnter(Entity) {};
};