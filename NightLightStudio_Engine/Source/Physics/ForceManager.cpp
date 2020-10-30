#pragma once

#include <map>
#include "ForceManager.h"

namespace NS_PHYSICS
{

void ForceManager::addForce(int forceHandle, const TranslationalForce& transForce)
{
	// check if force handle is uninit/unassigned
	if (forceHandle == -1)
	{
		int n = 0;
		while (translationalForceMap.find(currentKeyIndex) != translationalForceMap.end()) 
		{
			// found
			// incremnet and hope that it is free
			++currentKeyIndex;

			++n;
			if (n == 1000) throw; // throws after n tries so it wun be stuck in a while loop
		}
		// not found
		// assignKey to handle
		forceHandle = currentKeyIndex;
		// increment the key index so the next key can be assigned
		++currentKeyIndex;
	}
	translationalForceMap.insert(std::make_pair(forceHandle, transForce));
}

void ForceManager::addForce(const ComponentRigidBody& rigidBody, const TranslationalForce& transForce)
{
	addForce(rigidBody.forceHandle, transForce);
}

void ForceManager::addForce(const ComponentRigidBody& rigidBody, NlMath::Vector3D direction, float magnitude, size_t time)
{
	addForce(rigidBody, TranslationalForce(direction, magnitude, time));
}

void ForceManager::updateTranslationalForces()
{
	auto itr = translationalForceMap.begin();
	auto itrEnd = translationalForceMap.end();

	while (itr != itrEnd)
	{
		//TranslationalForce force = (*itr).second;
		if (!(*itr).second.checkAlive())
		{
			// dead
			itr = translationalForceMap.erase(itr);
			//itrEnd = translationalForceMap.end(); // 
		}
		else
		{
			++itr;
		}
	}
}

NlMath::Vector3D ForceManager::resolveTranslationalForces(int forceHandle)
{
	if (forceHandle == -1) throw; // no force

	NlMath::Vector3D finalForce(0.0f, 0.0f, 0.0f);

	auto itr = translationalForceMap.lower_bound(forceHandle);
	auto itrEnd = translationalForceMap.upper_bound(forceHandle);

	while (itr != itrEnd)
	{
		//TranslationalForce force = (*itr).second;
		finalForce += (*itr).second.direction * (*itr).second.magnitute;
		++itr;
	}

	return finalForce;
}

NlMath::Vector3D ForceManager::resolveTranslationalForces(const ComponentRigidBody& rigidBody)
{
	int forceHandle = rigidBody.forceHandle;
	if (forceHandle == -1)
	{
		return NlMath::Vector3D(0.0f, 0.0f, 0.0f); // no forces acting on rb
	}
	return resolveTranslationalForces(forceHandle);
}




//void ForceManager::addMomentum(const TranslationalMomentum& transMom)
//{
//	translationalMomentumList.push_back(transMom);
//}

void ForceManager::addForce(const RotationalForce& rotForce)
{
	rotationalForceList.push_back(rotForce);
}

void ForceManager::addMomentum(const RotationalMomentum& rotMom)
{
	rotationalMomentumList.push_back(rotMom);
}

NlMath::Vector3D ForceManager::solveRotation()
{
	NlMath::Vector3D totalAngularVelocity(0);
	for (RotationalMomentum& rotMom : rotationalMomentumList)
	{
		float inertia = rotMom.mass * rotMom.radius;
		NlMath::Vector3D angularVelocity = rotMom.angularMomentum / inertia;
		totalAngularVelocity += angularVelocity;
	}
	//use to set rotation for transform
	return totalAngularVelocity;
}

} // NS
