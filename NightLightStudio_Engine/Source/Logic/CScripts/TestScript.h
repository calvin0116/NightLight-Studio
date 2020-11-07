#pragma once
// Temporary for C++ scripting
// Always inherit this for scripting!
#include "IScript.h"
// Testing
#include <iostream>
#include "..\..\Messaging\SystemBroadcaster.h"
#include "..\..\Messaging\Messages\MessageScriptRequest.h"
#include "TestScript2.h"

class TestScript : public IScript
{
  TestScript2* testScript2;
public:

  virtual void Init()
  {
    std::cout << "Init from TestScript" << std::endl;
    // Pass in entity name that script is attached to !! Name must be unique, duplicate names will result in getting the one with lower Entity ID !!
    MessageScriptRequest msg(101);
    // Process the request
    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(msg);
    // Get script from msg.
    testScript2 = dynamic_cast<TestScript2*>(msg._pScript);
  }

  virtual void Update()
  {
    //std::cout << "Update from TestScript" << std::endl;
    if (testScript2)
      testScript2->TestCallFromOtherScript();
  }

  virtual void Exit()
  {
    std::cout << "Exit from TestScript" << std::endl;
  }

  virtual void OnCollisionEnter(Entity other)
  {
    // Your own ID
    (void)MyID;
    // The other entity I collided with
    (void)other;
    std::cout << "OnCollisionEnter from TestScript" << std::endl;
  }

};