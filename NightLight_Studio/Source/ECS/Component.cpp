/***************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : Component.cpp
Author : Eric Aw Yuh Liang
DP email : yuhliangeric.a@digipen.edu
Course : GAM200F19-A
Brief Description :
	Contains all data members and member functions that are part of
	a Component, that will be attached to an Entity.
**************************************************************************/
#include "Component.h"


bool Component::GetUpdateStatus()
{
	return is_updateable;
}

void Component::SetUpdateStatus(bool status)
{
	is_updateable = status;
}


uint16_t Component::GetComponentID() const
{
	return component_ID;
}

void Component::SetComponentID(uint16_t ID)
{
	component_ID = ID;
}


void Component::SetComponentName(std::string Name)
{
	component_name = Name;
}


std::string Component::GetComponentName()
{
	return component_name;
}


Entity* Component::GetOwner() const
{
 	return owner;
}


void Component::SetOwner(Entity* set_owner)
{
	owner = set_owner;
}
