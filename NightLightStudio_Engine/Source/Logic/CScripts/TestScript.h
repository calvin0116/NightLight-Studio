#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"
// Testing
#include <iostream>

class TestScript : public IScript
{
public:
  virtual void Init()
  {
    std::cout << "Init from TestScript" << std::endl;
  }

  virtual void Update()
  {
    std::cout << "Update from TestScript" << std::endl;
  }

  virtual void Exit()
  {
    std::cout << "Exit from TestScript" << std::endl;
  }

  virtual void OnCollisionEnter()
  {
    std::cout << "OnCollisionEnter from TestScript" << std::endl;
  }

};