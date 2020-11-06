#pragma once
// Temporary for C++ scripting

class IScript
{
public:
  virtual void Init() {};
  virtual void Update() {};
  virtual void Exit() {};

  virtual void OnCollisionEnter() {};
};