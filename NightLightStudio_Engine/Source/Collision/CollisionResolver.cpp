#pragma once
#include "CollisionResolver.h"
#include "..\Physics\SystemPhysics.h"
#include <limits>

#include "..\Logic\SystemLogic.h"

#include "..\Graphics\CameraSystem.h"
#include "../Input/SystemInput.h"

void CollisionResolver()
{

}

void CollsionResolver::addCollisionEvent(const CollisionEvent& newEvent)
{
	colEventList.push_back(newEvent);
}

void CollsionResolver::addTriggerEvent(const TriggerEvent& newEvent)
{
	trigEventList.push_back(newEvent);
}

void CollsionResolver::addTriggerEvent(const Entity& object1, const Entity& object2)
{
	TriggerEvent newEvent(object1, object2);
	trigEventList.push_back(newEvent);
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
					resolveAABB(colEvent);

					//if (colEvent.rigid1->velocity.z > 0)
					//	colEvent.rigid1->velocity.z = 0;
					//if (colEvent.rigid2->velocity.z > 0)
					//	colEvent.rigid2->velocity.z = 0;
					break;
				}
				case SIDES::BACK:
				{
					colEvent.collisionNormal = NlMath::Vector3D(0.0f, 0.0f, 1.0f);
					resolveAABB(colEvent);

					//if (colEvent.rigid1->velocity.z < 0)
					//	colEvent.rigid1->velocity.z = 0;
					//if (colEvent.rigid2->velocity.z < 0)
					//	colEvent.rigid2->velocity.z = 0;
					break;
				}
				case SIDES::TOP:
				{
					colEvent.collisionNormal = NlMath::Vector3D(0.0f, 1.0f, 0.0f);
					resolveAABB(colEvent);


					//if (colEvent.rigid1->velocity.y > 0)
					//	colEvent.rigid1->velocity.y = 0;
					//if (colEvent.rigid2->velocity.y > 0)
					//	colEvent.rigid2->velocity.y = 0;
					break;
				}
				case SIDES::BOTTOM:
				{
					colEvent.collisionNormal = NlMath::Vector3D(0.0f, -1.0f, 0.0f);
					resolveAABB(colEvent);


					//if (colEvent.rigid1->velocity.y < 0)
					//	colEvent.rigid1->velocity.y = 0;
					//if (colEvent.rigid2->velocity.y < 0)
					//	colEvent.rigid2->velocity.y = 0;
					break;
				}
				case SIDES::RIGHT:
				{
					colEvent.collisionNormal = NlMath::Vector3D(1.0f, 0.0f, 0.0f);
					resolveAABB(colEvent);


					//if (colEvent.rigid1->velocity.x > 0)
					//	colEvent.rigid1->velocity.x = 0;
					//if (colEvent.rigid2->velocity.x > 0)
					//	colEvent.rigid2->velocity.x = 0;
					break;
				}
				case SIDES::LEFT:
				{
					colEvent.collisionNormal = NlMath::Vector3D(-1.0f, 0.0f, 0.0f);
					resolveAABB(colEvent);


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



	// Resolve Logic events

	// SYS LOGIC TODO!!!

	for (TriggerEvent& trigEvent : trigEventList)
	{
		///////////////////////////////

		// pass to syslogic
		NS_LOGIC::SYS_LOGIC->OnTriggerEnter(trigEvent.entity1, trigEvent.entity2);



/////////////////////////////dun touch///////////////////////////////
		////colEvent.entity1;
		////colEvent.entity2;
		//bool flag = trigEvent.entity1.getComponent<ColliderComponent>()->triggerFlag;
		//bool prevFlag = trigEvent.entity1.getComponent<ColliderComponent>()->prevTriggerFlag;
		////on trigger here
		//if (flag)
		//{
		//	//NS_LOGIC::SYS_LOGIC->OnTriggerEnter(colEvent.entity1, colEvent.entity2);
		//}
		//else if (flag||prevFlag)
		//{
		//	//NS_LOGIC::SYS_LOGIC->OnTriggerEnter(colEvent.entity1, colEvent.entity2);
		//}
		//else
		//{
		//	//NS_LOGIC::SYS_LOGIC->OnTriggerexit(colEvent.entity1, colEvent.entity2);
		//}
/////////////////////////////dun touch///////////////////////////////
	}

	for (CollisionEvent& colEvent : colEventList)
	{
		//colEvent.entity1;
		//colEvent.entity2;

		// pass to syslogic
		NS_LOGIC::SYS_LOGIC->OnCollisionEnter(colEvent.entity1, colEvent.entity2);
	}



	// clear event list
	// collision events should be cleared before collision detection
	//colEventList.clear();
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

	//
	
	//Entity e = G_ECMANAGER->getEntity("hello");
	//

	//NlMath::Vector3D entity1Force = NS_PHYSICS::USE_THE_FORCE.resolveTranslationalForces(colEvent.entity1);
	//NlMath::Vector3D entity2Force = NS_PHYSICS::USE_THE_FORCE.resolveTranslationalForces(colEvent.entity2);
	NlMath::Vector3D entity1Force = colEvent.rigid1->force;
	NlMath::Vector3D entity2Force = colEvent.rigid2->force;
	float totalFriction = colEvent.rigid1->friction + colEvent.rigid2->friction;

	if (!colEvent.rigid1->isStatic)
	{
		float dot1 = colEvent.collisionNormal * colEvent.rigid1->velocity;
		colEvent.rigid1->velocity -= colEvent.collisionNormal * dot1;

		float dot1a = colEvent.collisionNormal * colEvent.rigid1->acceleration;
		colEvent.rigid1->acceleration -= colEvent.collisionNormal * dot1a;

		float dot1f = colEvent.collisionNormal * entity1Force;
		NlMath::Vector3D normalForce = colEvent.collisionNormal * dot1f;
		normalForce = normalForce * -1;
		
		// normal reaction force
		NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(colEvent.entity1, normalForce, 1.0f);


		// F = m a
		// F = m dp / dt

		// force due to the other object
		// Use momentum not force !!

		// m1 v1 + m2 v2 = m1f v1f + m2f v2f

		//NlMath::Vector3D externalForce = entity2Force;

		//float dote = colEvent.collisionNormal * externalForce;
		//NlMath::Vector3D externalForceResolve = colEvent.collisionNormal * dote;

		//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(colEvent.entity1, externalForceResolve, 1.0f);





		// frictional force = friction co-efficient * normal force

		float frictionalForce_mag = (totalFriction * normalForce).length();
		if (colEvent.rigid1->velocity.x != 0.0f || colEvent.rigid1->velocity.y != 0.0f || colEvent.rigid1->velocity.z != 0.0f)
		{
			NlMath::Vector3D frictionalForce = colEvent.rigid1->velocity * -1;
			frictionalForce = NlMath::Vector3DNormalize(frictionalForce);
			frictionalForce = frictionalForce * frictionalForce_mag /*+ 
				colEvent.rigid1->friction * 
				colEvent.rigid1->mass * colEvent.rigid1->mass *
				colEvent.rigid1->velocity * -1*/;

			NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(colEvent.entity1, frictionalForce, 1.0f);
		}
	}
	if (!colEvent.rigid2->isStatic)
	{
		NlMath::Vector3D oppNorm = colEvent.collisionNormal * -1;

		float dot2 = oppNorm * colEvent.rigid2->velocity;
		colEvent.rigid2->velocity -= oppNorm * dot2;

		float dot2a = oppNorm * colEvent.rigid2->acceleration;
		colEvent.rigid2->acceleration -= oppNorm * dot2a;

		float dot2f = oppNorm * entity2Force;
		NlMath::Vector3D normalForce = oppNorm * dot2f;
		normalForce = normalForce * -1;

		// normal reaction force
		NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(colEvent.entity2, normalForce, 1.0f);



		//// force due to the other object
		//NlMath::Vector3D externalForce = entity1Force;

		//float dote = oppNorm * externalForce;
		//NlMath::Vector3D externalForceResolve = oppNorm * dote;

		//NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(colEvent.entity2, externalForceResolve, 1.0f);





		// frictional force = friction co-efficient * normal force

		float frictionalForce_mag = (totalFriction * normalForce).length();

		if (colEvent.rigid2->velocity.x != 0.0f || colEvent.rigid2->velocity.y != 0.0f || colEvent.rigid2->velocity.z != 0.0f)
		{
			NlMath::Vector3D frictionalForce = colEvent.rigid2->velocity * -1;
			frictionalForce = NlMath::Vector3DNormalize(frictionalForce);
			frictionalForce = frictionalForce * frictionalForce_mag /*+ 
				colEvent.rigid2->friction * 
				colEvent.rigid2->mass * colEvent.rigid1->mass *
				colEvent.rigid2->velocity * -1*/;

			NS_PHYSICS::USE_THE_FORCE.addForceToNextTick(colEvent.entity2, frictionalForce, 1.0f);
		}
	}
}

void CollsionResolver::resolveAABB(const CollisionEvent& colEvent)
{
	resolveEventNormally(colEvent);

	switch (colEvent.colidingSide)
	{
	case SIDES::NO_COLLISION:
	{
		// no collision y u here
		throw;
		break;
	}
	case SIDES::FRONT:
	{
		float pen = colEvent.collider2->collider.aabb.vecMin.z - colEvent.collider1->collider.aabb.vecMax.z;

		pen = pen * 0.5f;

		if (!colEvent.rigid2->isStatic)
		{
			colEvent.transform2->_position.z += -pen - std::numeric_limits<float>::epsilon();
		}

		if (!colEvent.rigid1->isStatic)
		{
			colEvent.transform1->_position.z += pen + std::numeric_limits<float>::epsilon();
		}
		break;
	}
	case SIDES::BACK:
	{
		float pen = colEvent.collider1->collider.aabb.vecMin.z - colEvent.collider2->collider.aabb.vecMax.z;

		pen = pen * 0.5f;

		if (!colEvent.rigid1->isStatic)
		{
			colEvent.transform1->_position.z += -pen - std::numeric_limits<float>::epsilon();
		}

		if (!colEvent.rigid2->isStatic)
		{
			colEvent.transform2->_position.z += pen + std::numeric_limits<float>::epsilon();
		}

		break;
	}
	case SIDES::TOP:
	{
		float pen = colEvent.collider2->collider.aabb.vecMin.y - colEvent.collider1->collider.aabb.vecMax.y;
		if (!colEvent.rigid2->isStatic)
		{
			colEvent.transform2->_position.y += -pen;
		}

		if (!colEvent.rigid1->isStatic)
		{
			//colEvent.transform1->_position.y = colEvent.transform2->_position.y - pen;
		}
		break;
	}
	case SIDES::BOTTOM:
	{
		float pen = colEvent.collider1->collider.aabb.vecMin.y - colEvent.collider2->collider.aabb.vecMax.y;
		if (!colEvent.rigid1->isStatic)
		{
			colEvent.transform1->_position.y += -pen;
		}

		if (!colEvent.rigid2->isStatic)
		{
			//colEvent.transform2->_position.y = colEvent.transform1->_position.y - pen;
		}
		break;
	}
	case SIDES::RIGHT:
	{
		float pen = colEvent.collider2->collider.aabb.vecMin.x - colEvent.collider1->collider.aabb.vecMax.x;

		pen = pen * 0.5f;

		if (!colEvent.rigid2->isStatic)
		{
			colEvent.transform2->_position.x += -pen - std::numeric_limits<float>::epsilon();
		}

		if (!colEvent.rigid1->isStatic)
		{
			colEvent.transform1->_position.x += pen + std::numeric_limits<float>::epsilon();
		}
		break;
	}
	case SIDES::LEFT:
	{
		float pen = colEvent.collider1->collider.aabb.vecMin.x - colEvent.collider2->collider.aabb.vecMax.x;

		pen = pen * 0.5f;

		if (!colEvent.rigid1->isStatic)
		{
			colEvent.transform1->_position.x += -pen - std::numeric_limits<float>::epsilon();
		}

		if (!colEvent.rigid2->isStatic)
		{
			colEvent.transform2->_position.x += pen + std::numeric_limits<float>::epsilon();
		}
		break;
	}
	default:
		// nani
		throw;
	}



}


