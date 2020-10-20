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
		//AABB resolution
		switch (colEvent.colidingSide)
		{
		case SIDES::NO_COLLISION:
		{
			break;
		}
		case SIDES::FRONT:
		{
			if (colEvent.rigid1->velocity.z > 0)
			colEvent.rigid1->velocity.z = 0;
			break;
		}
		case SIDES::BACK:
		{
			if (colEvent.rigid1->velocity.z < 0)
			colEvent.rigid1->velocity.z = 0;
			break;
		}
		case SIDES::TOP:
		{
			if (colEvent.rigid1->velocity.y > 0)
			colEvent.rigid1->velocity.y = 0;
			break;
		}
		case SIDES::BOTTOM:
		{
			if (colEvent.rigid1->velocity.y < 0)
			colEvent.rigid1->velocity.y = 0;
			break;
		}
		case SIDES::RIGHT:
		{
			if (colEvent.rigid1->velocity.x > 0)
			colEvent.rigid1->velocity.x = 0;
			break;
		}
		case SIDES::LEFT:
		{
			if (colEvent.rigid1->velocity.x < 0)
			colEvent.rigid1->velocity.x = 0;
			break;
		}
		default:
			throw;
		}
	}
	colEventList.clear();
}

void CollsionResolver::clear()
{
	colEventList.clear();
}


