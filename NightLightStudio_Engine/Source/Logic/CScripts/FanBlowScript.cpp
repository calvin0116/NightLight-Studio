#pragma once
// Temporary for C++ scripting
#include "FanBlowScript.h"
#include "..\..\Core\DeltaTime.h"

// Player script to get their state
#include "../../Component/ComponentCScript.h"
#include "../../Physics/SystemPhysics.h"
#include "../../Component/ComponentCollider.h"

void FanBlowScript::Init()
{
  //Talisman1ID = Talisman1.getId();
  //ComponentCollider* colComp = MyID.getComponent<ComponentCollider>();
  //colComp->isTrigger = true;
  //std::cout << "Turned off Col for Entity: " << MyID.getId() << std::endl;
  //colComp->isCollidable = false;
}

void FanBlowScript::LateInit()
{
  Talisman1 = G_ECMANAGER->getEntity(talismanName);
}

void FanBlowScript::Update()
{

}

void FanBlowScript::Exit()
{

}

void FanBlowScript::OnCollisionEnter(Entity other)
{
  //std::cout << "Triggered" << std::endl;
}

void FanBlowScript::OnTriggerEnter(Entity other)
{
  if (Activate && other.getId() == Talisman1.getId())
    NS_PHYSICS::USE_THE_FORCE.addForce(Talisman1, Direction, Magnitude);
}