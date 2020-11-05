#pragma once
#include "CollisionResolver.h"
#include "..\Physics\SystemPhysics.h"

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
					colEvent.collisionNormal = NlMath::Vector3D(0.0f, 0.0f, -1.0f);
					AABBResolve(colEvent);

					//if (colEvent.rigid1->velocity.z > 0)
					//	colEvent.rigid1->velocity.z = 0;
					//if (colEvent.rigid2->velocity.z > 0)
					//	colEvent.rigid2->velocity.z = 0;
					break;
				}
				case SIDES::BACK:
				{
					colEvent.collisionNormal = NlMath::Vector3D(0.0f, 0.0f, 1.0f);
					AABBResolve(colEvent);

					//if (colEvent.rigid1->velocity.z < 0)
					//	colEvent.rigid1->velocity.z = 0;
					//if (colEvent.rigid2->velocity.z < 0)
					//	colEvent.rigid2->velocity.z = 0;
					break;
				}
				case SIDES::TOP:
				{
					colEvent.collisionNormal = NlMath::Vector3D(0.0f, 1.0f, 0.0f);
					AABBResolve(colEvent);


					//if (colEvent.rigid1->velocity.y > 0)
					//	colEvent.rigid1->velocity.y = 0;
					//if (colEvent.rigid2->velocity.y > 0)
					//	colEvent.rigid2->velocity.y = 0;
					break;
				}
				case SIDES::BOTTOM:
				{
					colEvent.collisionNormal = NlMath::Vector3D(0.0f, -1.0f, 0.0f);
					AABBResolve(colEvent);


					//if (colEvent.rigid1->velocity.y < 0)
					//	colEvent.rigid1->velocity.y = 0;
					//if (colEvent.rigid2->velocity.y < 0)
					//	colEvent.rigid2->velocity.y = 0;
					break;
				}
				case SIDES::RIGHT:
				{
					colEvent.collisionNormal = NlMath::Vector3D(1.0f, 0.0f, 0.0f);
					AABBResolve(colEvent);


					//if (colEvent.rigid1->velocity.x > 0)
					//	colEvent.rigid1->velocity.x = 0;
					//if (colEvent.rigid2->velocity.x > 0)
					//	colEvent.rigid2->velocity.x = 0;
					break;
				}
				case SIDES::LEFT:
				{
					colEvent.collisionNormal = NlMath::Vector3D(-1.0f, 0.0f, 0.0f);
					AABBResolve(colEvent);


					//if (colEvent.rigid1->velocity.x < 0)
					//	colEvent.rigid1->velocity.x = 0;
					//if (colEvent.rigid2->velocity.x < 0)
					//	colEvent.rigid2->velocity.x = 0;
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
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::AABB_CAPSULE:
			{
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::SPHERE_AABB:
			{
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::SPHERE_SPHERE:
			{
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::SPHERE_CAPSULE:
			{
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::CAPSULE_AABB:
			{
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::CAPSULE_SPHERE:
			{
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::CAPSULE_CAPSULE:
			{
				AABBResolve(colEvent);
				break;
			}
			case COLRESTYPE::OBB_OBB:
			{
				AABBResolve(colEvent);
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

void CollsionResolver::AABBResolve(const CollisionEvent& colEvent)
{
	// ref: https://stackoverflow.com/questions/5060082/eliminating-a-direction-from-a-vector
	// "Calculate the dot product of the geometry wall normal with the velocity vector of the object. 
	//    The result equals the velocity component in the direction of the wall normal. Subtract the wall 
	//    normal multiplied by this result from the velocity vector to remove all velocity in that direction. " 
	if (!colEvent.rigid1->isStatic)
	{
		float dot1 = colEvent.collisionNormal * colEvent.rigid1->velocity;
		colEvent.rigid1->velocity -= colEvent.collisionNormal * dot1;

		float dot1a = colEvent.collisionNormal * colEvent.rigid1->acceleration;
		colEvent.rigid1->acceleration -= colEvent.collisionNormal * dot1a;

		if (colEvent.rigid1->force.y > 10)
		{
			int i = 1;
		}

		float dot1f = colEvent.collisionNormal * colEvent.rigid1->force;
		NlMath::Vector3D normalForce = colEvent.collisionNormal * dot1f;
		normalForce = normalForce * -1;

		//NS_PHYSICS::SYS_PHYSICS->getForceManager()->addForce(*colEvent.rigid1, colEvent.collisionNormal, 7);
		NS_PHYSICS::USE_THE_FORCE.addForce(*colEvent.rigid1, normalForce, 1.0f);
		
	}
	if (!colEvent.rigid2->isStatic)
	{
		NlMath::Vector3D oppNorm = colEvent.collisionNormal * -1;

		float dot2 = oppNorm * colEvent.rigid2->velocity;
		colEvent.rigid2->velocity -= oppNorm * dot2;

		float dot2a = colEvent.collisionNormal * colEvent.rigid2->acceleration;
		colEvent.rigid2->acceleration -= oppNorm * dot2a;

		float dot2f = oppNorm * colEvent.rigid2->force;
		NlMath::Vector3D normalForce = oppNorm * dot2f;
		normalForce = normalForce * -1;

		//NS_PHYSICS::SYS_PHYSICS->getForceManager()->addForce(*colEvent.rigid1, colEvent.collisionNormal, 7);
		NS_PHYSICS::USE_THE_FORCE.addForce(*colEvent.rigid2, normalForce, 1.0f);
	}
}


