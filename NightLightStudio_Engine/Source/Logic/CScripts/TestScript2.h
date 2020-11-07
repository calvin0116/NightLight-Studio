#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"
// Testing
#include <iostream>

class TestScript2 : public IScript
{
public:

  virtual void Init()
  {
  }

  virtual void Update()
  {
    //std::cout << "Update from TestScript" << std::endl;
  }

  virtual void Exit()
  {
  }

  virtual void OnCollisionEnter(Entity other)
  {
    // Your own ID
    (void)MyID;
    // The other entity I collided with
    (void)other;
    std::cout << "OnCollisionEnter from TestScript" << std::endl;
  }

  virtual void TestCallFromOtherScript()
  {
    std::cout << "Hello from TestScript2!" << std::endl;
  }
};