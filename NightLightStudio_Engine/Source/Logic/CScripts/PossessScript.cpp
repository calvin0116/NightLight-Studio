#pragma once
// Temporary for C++ scripting
#include "PossessScript.h"

void PossessScript::Init()
{
  // Fetch player entity here.
  PlayerEntity = G_ECMANAGER->getEntity(1);
}

void PossessScript::Update()
{
  
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