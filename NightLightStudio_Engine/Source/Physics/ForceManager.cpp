#pragma once

#include <map>
#include "ForceManager.h"
#include "..\Component\ComponentManager.h"

namespace NS_PHYSICS
{


void ForceManager::addForce(Entity ent, const TranslationalForce& transForce)
{
	// check if force handle is uninit/unassigned
	// use ent id instead

	if (ent.getId() == -1)
	{
		//std::cout << "add Force Error" << std::endl;
		// throw;
	}
	translationalForceMap.insert(std::pair<int, TranslationalForce>(ent.getId(), transForce));
}

void ForceManager::addForce(Entity ent, NlMath::Vector3D direction, float magnitude)
{
	addForce(ent, TranslationalForce(direction, magnitude));
}

void ForceManager::addForceToNextTick(Entity ent, NlMath::Vector3D direction, float magnitude)
{
	if (ent.getId() == -1)
	{
		//std::cout << "add Force Error" << std::endl;
		// throw;
	}
	translationalForceMap_next.insert(std::pair<int, TranslationalForce>(ent.getId(), TranslationalForce(direction, magnitude)));
}

void ForceManager::updateTranslationalForces()
{
	for (auto element : translationalForceMap_next)
	{
		translationalForceMap.insert(element);
	}
}

void ForceManager::clearTranslationalForces()
{
	translationalForceMap.clear();
	translationalForceMap_next.clear();
}

NlMath::Vector3D ForceManager::resolveTranslationalForces(int entityId)
{
	if (entityId == -1) throw; // no force

	NlMath::Vector3D finalForce(0.0f, 0.0f, 0.0f);

	auto itr = translationalForceMap.lower_bound(entityId);
	auto itrEnd = translationalForceMap.upper_bound(entityId);

	while (itr != itrEnd)
	{
		//TranslationalForce force = (*itr).second;
		finalForce += (*itr).second.direction * (*itr).second.magnitute;
		++itr;
	}

	return finalForce;
}

NlMath::Vector3D ForceManager::resolveTranslationalForces(Entity ent)
{
	if (ent.getId() == -1)
	{
		//std::cout <<  "Force resoultion Error" << std::endl;
		//throw;
	}
	return resolveTranslationalForces(ent.getId());
}

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
