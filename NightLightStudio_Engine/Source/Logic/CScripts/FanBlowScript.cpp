#pragma once
// Temporary for C++ scripting
#include "FanBlowScript.h"
#include "..\..\Core\DeltaTime.h"

// Player script to get their state
#include "../../Component/ComponentCScript.h"
#include "../../Physics/SystemPhysics.h"

void FanBlowScript::Init()
{
  Talisman1 = G_ECMANAGER->getEntity(talismanName);
  //Talisman1ID = Talisman1.getId();
}

void FanBlowScript::Update()
{

}

void FanBlowScript::Exit()
{

}

void FanBlowScript::OnCollisionEnter(Entity other)
{
}

void FanBlowScript::OnTriggerEnter(Entity other)
{
  if (Activate && other.getId() == Talisman1.getId())
    NS_PHYSICS::USE_THE_FORCE.addForce(Talisman1, Direction, Magnitude);
}