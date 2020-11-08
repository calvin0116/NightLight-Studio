#pragma once
// Temporary for C++ scripting
#include "PossessScript.h"
#include "..\..\Core\DeltaTime.h"
#include "../../Component/ComponentCScript.h"


void PossessScript::Init()
{
  // Fetch player entity here.
    playerTransform = nullptr;
    

}

void PossessScript::Update()
{
  //ChangeLight(10.0f);

}

void PossessScript::Exit()
{

}

void PossessScript::OnCollisionEnter(Entity other)
{
  //ComponentCScript* tmp = other.getComponent<ComponentCScript>();
  //if (tmp != nullptr)
  //{
  //    if (tmp->_iTag == 0)
	 // {
  //        //logic
  //        player = dynamic_cast<Player*>(tmp->_pScript);
  //        //if the colliding player is in butterfly state
  //        if (player->getState() == PLAYERSTATE::BUTTERFLY)
  //        {
  //            //it will possess the object
  //            player->changeState(PLAYERSTATE::POSSESSED);
  //        }
  //       
  //        //change player transform here
  //        if (playerTransform == nullptr)
  //        {
  //            playerTransform = other.getComponent<ComponentTransform>();
  //            *playerTransform = *MyID.getComponent<ComponentTransform>();
  //            //turn off own collider
  //            MyID.getComponent<ComponentCollider>()->isCollidable = false;

  //            
  //        }
	 // }
  //}
}

void PossessScript::ChangeLight(float val)
{
  if (Possessed && LightIntensity < MaxIntensity)
  {
    val = fabsf(val);
    LightIntensity += val * DELTA_T->dt;
  }
  else if (!Possessed && LightIntensity > MinIntensity)
  {
    val = fabsf(val);
    LightIntensity += -val * DELTA_T->dt;
  }
}

void PossessScript::Test()
{
  std::cout << "Hello from Possess Script" << std::endl;
}