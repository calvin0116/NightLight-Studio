#pragma once
// Temporary for C++ scripting
#include "PossessScript.h"
#include "APIScript.h"
#include "..\..\Core\DeltaTime.h"

void PossessScript::Init()
{
  // Fetch player entity here.
  // GetEntity by name
  PlayerEntity = APIScript::FindEntityByName("Player");
  CompLight = PlayerEntity.getComponent<ComponentLight>();
}

void PossessScript::Update()
{
  ChangeLight(10.0f);
}

void PossessScript::Exit()
{

}

void PossessScript::OnCollisionEnter(Entity other)
{
  // Your own ID
  (void)MyID;
  // The other entity I collided with
  (void)other;

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