#pragma once
#include "CollisionResolver.h"
#include "..\Physics\SystemPhysics.h"
#include <limits>

#include "..\Logic\SystemLogic.h"

#include "..\Graphics\CameraSystem.h"
#include "../Input/SystemInput.h"
#include "../Core/DeltaTime.h"

#define BAUMGARTE 0.2f;
#define PENETRATION_SLOP 0.05f;

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
				//init

				init(colEvent);
				preSolveOBB(colEvent);
				for (size_t i = 0; i < _iteration; i++)
				{
					solveOBB(colEvent);
				}
				//remove the event after it is solved
				//colEventList.remove(colEvent);
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


	//auto itr = colEventList.begin();
	//auto itrEnd = colEventList.end();

	//while (itr != itrEnd)
	//{
	//	//colEvent.entity1;
	//	//colEvent.entity2;

	//	// pass to syslogic

	//	CollisionEvent& colEvent = *itr;

	//	if (colEvent.collidingFrames == 0)
	//	{
	//		NS_LOGIC::SYS_LOGIC->OnCollisionEnter(colEvent.entity1, colEvent.entity2);
	//	}

	//	else if (colEvent.collidingFrames < COLLIDING_FRAMES_TRASHOLD)
	//	{
	//		NS_LOGIC::SYS_LOGIC->OnCollisionStay(colEvent.entity1, colEvent.entity2);
	//		++colEvent.collidingFrames;
	//	}
	//	else
	//	{
	//		NS_LOGIC::SYS_LOGIC->OnCollisionExit(colEvent.entity1, colEvent.entity2);
	//		colEventList.erase(itr); //
	//	}
	//	++itr;
	//}


	// clear event list
	// collision events should be cleared before collision detection
	//colEventList.clear();
}

void CollsionResolver::clear()
{
	colEventList.clear();
  trigEventList.clear();
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

void CollsionResolver::ComputeBasis(const NlMath::Vec3& normal, NlMath::Vec3* tangentVec1, NlMath::Vec3* tangentVec2)
{
	// Suppose vector a has all equal components and is a unit vector: a = (s, s, s)
	// Then 3*s*s = 1, s = sqrt(1/3) = 0.57735027. This means that at least one component of a
	// unit vector must be greater or equal to 0.57735027. Can use SIMD select operation.

	if (fabs(normal.x) >= float(0.57735027))
		*tangentVec1 = NlMath::Vec3(normal.y, -normal.x, float(0.0));
	else
		*tangentVec1 = NlMath::Vec3(float(0.0), normal.z, -normal.y);

	*tangentVec1 = NlMath::Vector3DNormalize(*tangentVec1);
	*tangentVec2 = NlMath::Vector3DCrossProduct(normal, *tangentVec1);
}

void CollsionResolver::setIteration(unsigned int iteration)
{
	_iteration = max(1, iteration);
}

NlMath::Matrix4x4 CollsionResolver::ComputeIntertia(OBBCollider* collider, ComponentRigidBody* rigid)
{
	NlMath::Vector3D e = collider->extend;
	// Calculate inertia tensor
	float ex2 = float(4.0) * e.x * e.x;
	float ey2 = float(4.0) * e.y * e.y;
	float ez2 = float(4.0) * e.z * e.z; 
	float mass = rigid->mass;
	float x = float(1.0 / 12.0) * mass * (ey2 + ez2);
	float y = float(1.0 / 12.0) * mass * (ex2 + ez2);
	float z = float(1.0 / 12.0) * mass * (ex2 + ey2);
	NlMath::Matrix4x4 inertia;
	inertia.SetDiagonal(NlMath::Vec3(x, y, z));

	//// Transform tensor to local space
	//inertia = local.rotation * inertia * q3Transpose(local.rotation);
	//q3Mat3 identity;
	//q3Identity(identity);
	//inertia += (identity * q3Dot(local.position, local.position) - q3OuterProduct(local.position, local.position)) * mass;
	// collisionData md;
	//md->center = local.position;
	//md->inertia = inertia;

	//////////////////////for compound object//////////////////////////
	//NlMath::Matrix4x4 identity(1);
	//inertia -= (identity * /*dot product*/(lc* lc) - q3OuterProduct(lc, lc)) * mass;
	//m_invInertiaModel = q3Inverse(inertia);

	return inertia;

}

void CollsionResolver::preSolveOBB(CollisionEvent& colEvent)
{
		//storing all vectors
		NlMath::Vec3 velocityA = colEvent.rigid1->velocity;
		NlMath::Vec3 angularVelocityA = colEvent.rigid1->angularVelocity;
		NlMath::Vec3 velocityB = colEvent.rigid2->velocity;
		NlMath::Vec3 angularVelocityB = colEvent.rigid2->angularVelocity;

		//for all contact
		for (Contact& contact : colEvent.manifold.contacts)
		{

			// Vector from Center.Of.Mass to contact position
			NlMath::Vec3 ra = contact.position - colEvent.transform1->_position;
			NlMath::Vec3 rb = contact.position - colEvent.transform2->_position;

			// Precalculate JM^-1JT for contact and friction constraints
			NlMath::Vec3 raCn = NlMath::Vector3DCrossProduct(ra, colEvent.manifold.normal);
			NlMath::Vec3 rbCn = NlMath::Vector3DCrossProduct(rb, colEvent.manifold.normal);
			float nm = colEvent.rigid1->mass + colEvent.rigid2->mass;
			//tangent mass
			float tm[2];
			tm[0] = nm;
			tm[1] = nm;

			nm += raCn * ( colEvent.inertia1 * raCn) + rbCn * (colEvent.inertia2 * rbCn);
			//inverse the mass
			contact.normalMass= nm != 0.0f ? 1.0f / nm : 0.0f;

			for (unsigned int i = 0; i < 2; ++i)
			{
				NlMath::Vec3 raCt = NlMath::Vector3DCrossProduct(colEvent.manifold.tangentVectors[i], ra);
				NlMath::Vec3 rbCt = NlMath::Vector3DCrossProduct(colEvent.manifold.tangentVectors[i], rb);
				tm[i] += raCt* (colEvent.inertia1 * raCt) + rbCt* (colEvent.inertia2 * rbCt);
				//inverse the mass
				contact.tangentMass[i] = (tm[i] != 0.0f ? 1.0f / tm[i] : 0.0f);
			}

			// Precalculate bias factor
			float realDt = DELTA_T->dt / CLOCKS_PER_SEC;
			contact.bias = -0.2f * (1.0f / realDt) * min(float(0.0), contact.penetration + 0.05f);

			// Warm start contact
			NlMath::Vec3 P = colEvent.manifold.normal * contact.normalImpulse;

			//if (m_enableFriction)
			{
				P += colEvent.manifold.tangentVectors[0] * contact.tangentImpulse[0];
				P += colEvent.manifold.tangentVectors[1] * contact.tangentImpulse[1];
			}

			velocityA -= P * colEvent.rigid1->mass;
			angularVelocityA -= colEvent.inertia1 * NlMath::Vector3DCrossProduct(ra, P);

			velocityB += P * colEvent.rigid2->mass;
			angularVelocityB += colEvent.inertia2 * NlMath::Vector3DCrossProduct(rb, P);

			//////////////////restitution bias for bounciness/////////////////////////////////
			//// Add in restitution bias
			//float relativeVelocity = q3Dot(velocityB + q3Cross(angularVelocityB, c->rb) - velocityA - q3Cross(angularVelocityA, c->ra), cs->normal);

			//if (relativeVelocity < -float(1.0))
			//	c->bias += -(cs->restitution) * relativeVelocity;
		}

		colEvent.rigid1->velocity = velocityA;
		colEvent.rigid1->angularVelocity = angularVelocityA;
		colEvent.rigid2->velocity = velocityB;
		colEvent.rigid2->angularVelocity = angularVelocityB;
	
}

void CollsionResolver::solveOBB(CollisionEvent& colEvent)
{
	//storing all vectors
	NlMath::Vec3 velocityA = colEvent.rigid1->velocity;
	NlMath::Vec3 angularVelocityA = colEvent.rigid1->angularVelocity;
	NlMath::Vec3 velocityB = colEvent.rigid2->velocity;
	NlMath::Vec3 angularVelocityB = colEvent.rigid2->angularVelocity;

	//for all contact
	for (Contact& contact : colEvent.manifold.contacts)
	{
		// Vector from Center.Of.Mass to contact position
		NlMath::Vec3 ra = contact.position - colEvent.transform1->_position;
		NlMath::Vec3 rb = contact.position - colEvent.transform2->_position;
		// relative velocity at contact
		NlMath::Vec3 relativeVelocity = velocityB + NlMath::Vector3DCrossProduct(angularVelocityB, rb)
			- velocityA - NlMath::Vector3DCrossProduct(angularVelocityA, ra);

		// Friction, might add in boolean to toggle friction
		//if (m_enableFriction)
		{
			for (unsigned int i = 0; i < 2; ++i)
			{
				float lambda = -(relativeVelocity* colEvent.manifold.tangentVectors[i]) * contact.tangentMass[i];

				//mix friction
				float friction = colEvent.rigid1->friction* colEvent.rigid2->friction;

				// Calculate frictional impulse
				float maxLambda = friction * contact.normalImpulse;

				// Clamp frictional impulse
				float oldPT = contact.tangentImpulse[i];
				contact.tangentImpulse[i] = Clamp(-maxLambda, maxLambda, oldPT + lambda);
				lambda = contact.tangentImpulse[i] - oldPT;

				// Apply friction impulse
				NlMath::Vec3 impulse = colEvent.manifold.tangentVectors[i] * lambda;
				velocityA -= impulse * colEvent.rigid1->mass;
				angularVelocityA -= colEvent.inertia1 * NlMath::Vector3DCrossProduct(ra, impulse);

				velocityB += impulse * colEvent.rigid2->mass;
				angularVelocityB += colEvent.inertia2 * NlMath::Vector3DCrossProduct(rb, impulse);
			}
		}

		// Normal
		{
			relativeVelocity = velocityB + NlMath::Vector3DCrossProduct(angularVelocityB, rb) - 
				velocityA - NlMath::Vector3DCrossProduct(angularVelocityA, ra);

			// Normal impulse
			float vn = (relativeVelocity* colEvent.manifold.normal);

			// Factor in positional bias to calculate impulse scalar j
			float lambda = contact.normalMass * (-vn + contact.bias);

			// Clamp impulse
			float tempPN = contact.normalImpulse;
			contact.normalImpulse = max(tempPN + lambda, float(0.0));
			lambda = contact.normalImpulse - tempPN;

			// Apply impulse
			NlMath::Vec3 impulse = colEvent.manifold.normal * lambda;
			velocityA -= impulse * colEvent.rigid1->mass;
			angularVelocityA -= colEvent.inertia1 * NlMath::Vector3DCrossProduct(ra, impulse);

			velocityB += impulse * colEvent.rigid2->mass;
			angularVelocityB += colEvent.inertia2 * NlMath::Vector3DCrossProduct(rb, impulse);
		}
	}

	colEvent.rigid1->velocity = velocityA;
	colEvent.rigid1->angularVelocity = angularVelocityA;
	colEvent.rigid2->velocity = velocityB;
	colEvent.rigid2->angularVelocity = angularVelocityB;
}

float CollsionResolver::Clamp(float min, float max, float a)
{
	if (a < min)
		return min;

	if (a > max)
		return max;

	return a;
}

void CollsionResolver::init(CollisionEvent& colEvent)
{
	//for (Contact& contact : colEvent.manifold.contacts)
	//{
	//	ra = contact.position - colEvent.transform1->_position;
	//	rb = contact.position - colEvent.transform2->_position;
	//}
	colEvent.inertia1 = ComputeIntertia(&colEvent.collider1->collider.obb, colEvent.rigid1);
	colEvent.inertia2 = ComputeIntertia(&colEvent.collider2->collider.obb, colEvent.rigid2);


	Manifold* manifold = &colEvent.manifold;
	//shallow copy
	Manifold oldManifold = colEvent.manifold;
	NlMath::Vec3 oldTangent0 = oldManifold.tangentVectors[0];
	NlMath::Vec3 oldTangent1 = oldManifold.tangentVectors[1];

	ComputeBasis(manifold->normal, manifold->tangentVectors, manifold->tangentVectors + 1);

	for (unsigned int i = 0; i < colEvent.manifold.contactCount; ++i)
	{
		Contact* contact = manifold->contacts + i;
		contact->tangentImpulse[0] = contact->tangentImpulse[1] = contact->normalImpulse = float(0.0);
		unsigned char oldWarmStart = contact->warmStarted;
		contact->warmStarted = unsigned char(0);

		for (unsigned int j = 0; j < oldManifold.contactCount; ++j)
		{
			//old contact
			Contact* oldContact = oldManifold.contacts + j;
			if (contact->fp.key == oldContact->fp.key)
			{
				contact->normalImpulse = oldContact->normalImpulse;

				// Attempt to re-project old friction solutions
				NlMath::Vec3 friction = oldTangent0 * oldContact->tangentImpulse[0] + oldTangent1 * oldContact->tangentImpulse[1];
				contact->tangentImpulse[0] = friction * manifold->tangentVectors[0];
				contact->tangentImpulse[1] = friction * manifold->tangentVectors[1];
				contact->warmStarted = max(oldWarmStart, unsigned char(oldWarmStart + 1));
				break;
			}
		}
	}
}


