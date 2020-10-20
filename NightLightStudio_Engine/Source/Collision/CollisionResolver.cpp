#pragma once
#include "CollisionResolver.h"

void CollisionResolver()
{

}

void CollsionResolver::addCollisionEvent(const CollisionEvent& _event)
{
	colEventList.push_back(_event);
}

void CollsionResolver::resolveCollision()
{
	for (CollisionEvent& colEvent : colEventList)
	{
		
	}
	colEventList.clear();
}

void CollsionResolver::clear()
{
	colEventList.clear();
}


