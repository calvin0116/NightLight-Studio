#pragma once
// Temporary for C++ scripting
#include "FanScript.h"
#include "..\..\Core\DeltaTime.h"

// Player script to get their state
#include "../Player.h"


void FanScript::Init()
{
  // Fetch player entity here.
  MyPlayerEntity = G_ECMANAGER->getEntity("Player");
  MyPlayerID = MyPlayerEntity.getId();
  //talisman1 = G_ECMANAGER->getEntity("Talisman_1");
  obb1 = G_ECMANAGER->getEntity("OBB_1");
}

void FanScript::Update()
{
  if (Activate)
  {
    if (timePassed >= wind_up_time)
    {
      Player* playerScript = reinterpret_cast<Player*>(playerScriptComp->_pScript);
      if (playerScript)
      {
        if (playerScript->getState() == PLAYERSTATE::HUMAN)
        {
          Activate = false;
          timePassed = 0.0f;
        }
        else
        {

        }
      }
    }
    else
      timePassed += DELTA_T->dt;
  }
}

void FanScript::Exit()
{

}

void FanScript::OnCollisionEnter(Entity other)
{
  if (other.getId() != MyPlayerID)
    return;
  if (playerScriptComp && playerScriptComp->_pScript)
  {
    Player* playerScript = reinterpret_cast<Player*>(playerScriptComp->_pScript);
    if (playerScript->getState() == PLAYERSTATE::MOTH)
    {
      playerScript->changeState(PLAYERSTATE::POSSESSED);
      Activate = true;
    }
  }
}

void FanScript::OnTriggerEnter(Entity other)
{
}