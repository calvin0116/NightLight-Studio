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

		switch (colEvent.collisionType)
		{
			case COLRESTYPE::AABB_AABB:
			{
				///////////////////////////////////////////////////////////////////////////////////
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
					if (colEvent.rigid2->velocity.z > 0)
						colEvent.rigid2->velocity.z = 0;
					break;
				}
				case SIDES::BACK:
				{
					if (colEvent.rigid1->velocity.z < 0)
						colEvent.rigid1->velocity.z = 0;
					if (colEvent.rigid2->velocity.z < 0)
						colEvent.rigid2->velocity.z = 0;
					break;
				}
				case SIDES::TOP:
				{
					if (colEvent.rigid1->velocity.y > 0)
						colEvent.rigid1->velocity.y = 0;
					if (colEvent.rigid2->velocity.y > 0)
						colEvent.rigid2->velocity.y = 0;
					break;
				}
				case SIDES::BOTTOM:
				{
					if (colEvent.rigid1->velocity.y < 0)
						colEvent.rigid1->velocity.y = 0;
					if (colEvent.rigid2->velocity.y < 0)
						colEvent.rigid2->velocity.y = 0;
					break;
				}
				case SIDES::RIGHT:
				{
					if (colEvent.rigid1->velocity.x > 0)
						colEvent.rigid1->velocity.x = 0;
					if (colEvent.rigid2->velocity.x > 0)
						colEvent.rigid2->velocity.x = 0;
					break;
				}
				case SIDES::LEFT:
				{
					if (colEvent.rigid1->velocity.x < 0)
						colEvent.rigid1->velocity.x = 0;
					if (colEvent.rigid2->velocity.x < 0)
						colEvent.rigid2->velocity.x = 0;
					break;
				}
				default:
					throw;
				}
				break;
				//AABB resolution END
				///////////////////////////////////////////////////////////////////////////////////
			}
			case COLRESTYPE::AABB_SPHERE:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::AABB_CAPSULE:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::SPHERE_AABB:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::SPHERE_SPHERE:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::SPHERE_CAPSULE:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::CAPSULE_AABB:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::CAPSULE_SPHERE:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::CAPSULE_CAPSULE:
			{
				resolveEventNormally(colEvent);
				break;
			}
			case COLRESTYPE::OBB_OBB:
			{
				resolveEventNormally(colEvent);
				break;
			}
			default:
				throw;
		}

	}
	// clear event list
	colEventList.clear();
}

void CollsionResolver::clear()
{
	colEventList.clear();
}

void CollsionResolver::resolveEventNormally(const CollisionEvent& colEvent)
{
	// ref: https://stackoverflow.com/questions/5060082/eliminating-a-direction-from-a-vector
	// "Calculate the dot product of the geometry wall normal with the velocity vector of the object. 
	//    The result equals the velocity component in the direction of the wall normal. Subtract the wall 
	//    normal multiplied by this result from the velocity vector to remove all velocity in that direction. " 
	float dot1 = colEvent.collisionNormal * colEvent.rigid1->velocity;
	colEvent.rigid1->velocity -= colEvent.collisionNormal * dot1;
	float dot2 = colEvent.collisionNormal * colEvent.rigid2->velocity;
	colEvent.rigid2->velocity -= colEvent.collisionNormal * dot2;
}


