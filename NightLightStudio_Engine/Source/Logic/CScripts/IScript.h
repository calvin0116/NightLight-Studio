#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "..\..\Component\ComponentManager.h" // E_CMANAGER

class IScript
{
protected:
  int  _Obj;
public:
  // IScript() {}
  virtual ~IScript() {}
  virtual void SetEntity(int _id) { _Obj = _id; }
  virtual void Init() {};
  virtual void Update() {};
  virtual void Exit() {};

  virtual void OnCollisionEnter(int other) {};
};