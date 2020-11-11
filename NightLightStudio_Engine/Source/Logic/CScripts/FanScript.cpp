#pragma once
// Temporary for C++ scripting
#include "FanScript.h"
#include "..\..\Core\DeltaTime.h"

// Components
#include "../../Component/ComponentCScript.h"

// Player script to get their state
#include "../Player.h"
#include "../CameraScript.h"
#include "FanBlowScript.h"


void FanScript::Init()
{
  // Fetch player entity here.
  MyPlayerEntity = G_ECMANAGER->getEntity(playerName);
  //MyPlayerID = MyPlayerEntity.getId();
  playerScriptComp = MyPlayerEntity.getComponent<ComponentCScript>();
  //talisman1 = G_ECMANAGER->getEntity("Talisman_1");
  obb1 = G_ECMANAGER->getEntity(obbName);
  obb1ScriptComp = obb1.getComponent<ComponentCScript>();

  CameraEntity = G_ECMANAGER->getEntity(camName);
  cameraScriptComp = CameraEntity.getComponent<ComponentCScript>();
}

void FanScript::Update()
{
  if (Activate)
  {
    if (timePassed >= wind_up_time)
    {
      Player* playerScript = nullptr;
      if(playerScriptComp)
        playerScript = reinterpret_cast<Player*>(playerScriptComp->_pScript);
      FanBlowScript* obb1Script = nullptr;
      if (obb1ScriptComp)
        obb1Script = reinterpret_cast<FanBlowScript*>(obb1ScriptComp->_pScript);
      if (playerScript)
      {
        if (playerScript->getState() == PLAYERSTATE::HUMAN)
        {
          Activate = false;
          timePassed = 0.0f;
        }
        if (obb1Script)
          obb1Script->Activate = Activate;
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
  if (other.getId() != MyPlayerEntity.getId())
    return;
  if (playerScriptComp && playerScriptComp->_pScript)
  {
    Player* playerScript = reinterpret_cast<Player*>(playerScriptComp->_pScript);
    AllScripts::CameraScript* camScript = reinterpret_cast<AllScripts::CameraScript*>(cameraScriptComp->_pScript);
    if (playerScript && camScript)
    {
      if (playerScript->getState() == PLAYERSTATE::MOTH)
      {
        //playerScript->changeState(PLAYERSTATE::POSSESSED);
        ComponentTransform* comTrans = MyID.getComponent<ComponentTransform>();
        camScript->SetTarget(comTrans->_position);
        camScript->SetDistance(playerScript->CAMERA_DISTANCE);
        //camScript->SetTargetOffsetXY(playerScript->CAMERA_OFFSET_X, playerScript->CAMERA_OFFSET_Y);
        //camScript->SetRotate(false);
        Activate = true;
      }
    }
  }
}

void FanScript::OnTriggerEnter(Entity other)
{
}