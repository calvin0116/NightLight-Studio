#pragma once
// Temporary for C++ scripting
#include "CauldronScript.h"
#include "..\..\Core\DeltaTime.h"

// Script
#include "../Player.h"

void CauldronScript::Init()
{
  PlayerENT = G_ECMANAGER->getEntityUsingEntName("Player");
  PlayerCOL = PlayerENT.getComponent<ComponentCollider>();
  PlayerCSCRIPT = PlayerENT.getComponent<ComponentCScript>();
}

void CauldronScript::Update()
{

}

void CauldronScript::Exit()
{

}

void CauldronScript::OnCollisionEnter(Entity other)
{

}

void CauldronScript::OnTriggerEnter(Entity other)
{
  if (PlayerENT.getId() == other.getId())
  {
    if (PlayerCSCRIPT->_pScript)
    {
      Player* script = reinterpret_cast<Player*>(PlayerCSCRIPT->_pScript);
      float realDt = DELTA_T->dt / CLOCKS_PER_SEC;
      if (script->_playerEnergy < script->PLAYER_MAX_ENERGY)
        script->_playerEnergy += realDt * script->PLAYER_ENERGY_REGEN;
    }
  }
}