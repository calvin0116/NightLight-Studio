#pragma once
#include "ForceManager.h"

void ForceManager::addForce(const TranslationalForce& transForce)
{
	translationalForceList.push_back(transForce);
}

void ForceManager::addMomentum(const TranslationalMomentum& transMom)
{
	translationalMomentumList.push_back(transMom);
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
