#pragma once
#include <vector>

#include <map>

#include <ctime> // for colock

#include "../Math/Vector.h"

#include "..\Component\ComponentRigidBody.h"

// force, F = dp / dt
// where p is momentum and t is time
// therefore, Force is the change in momentum over time
//
//
// change in momentum dp = F * dt // if im not wrong, change in moment dp = J, impulse
// Force * change in time
//
// momentum, p = mv
// where m is mass and v is velocity
// 
// F = d mv / dt
// if mass is constant, F = m dv/dt
// force becomes the change of velocity over time
//
// dv/dt, the change in velocity over time is accleration, a
// a = dv/dt
// therefore, force F = ma
// force is mass x accleration
//

namespace NS_PHYSICS
{





typedef struct TranslationalForce
{
	NlMath::Vector3D direction;
	float magnitute;  //magnitude
	std::clock_t aliveUntil;
	bool checkAlive()
	{
		if (aliveUntil < std::clock()) return false; return true; // xD
	}
	TranslationalForce() :
		direction(0.0f, 0.0f, 0.0f),
		magnitute(0.0f),
		aliveUntil(std::clock())
	{}
	TranslationalForce(NlMath::Vector3D dir, float mag, int t) :
		direction(dir),
		magnitute(mag),
		aliveUntil(t)
	{}
} Force;

//typedef struct TranslationalMomentum
//{
//	NlMath::Vector3D direction = 0;
//	float magnitute = 0;
//} TransMom;

typedef struct RotationalForce
{
	
} Torque;

typedef struct RotationalMomentum
{
	//L = MOR2
	float radius = 0;
	float mass = 0;
	NlMath::Vector3D angularMomentum;
}RotMom ;


class ForceManager
{
	//std::vector<TranslationalForce> translationalForceList;
	//std::vector<TranslationalMomentum> translationalMomentumList;

	std::vector<RotationalForce> rotationalForceList;
	std::vector<RotationalMomentum> rotationalMomentumList;


	// a map of forcehandle to translation force
	std::map<int, TranslationalForce> translationalForceMap;
	// current key assignemnt
	int currentKeyIndex;

public:
	// add force to the rigidbody 
	void addForce(ComponentRigidBody& rigidBody, const TranslationalForce& transForce);
	// add force to the rigidbody 
	void addForce(ComponentRigidBody& rigidBody, NlMath::Vector3D direction, float magnitude, size_t time = CLOCKS_PER_SEC);

	void updateTranslationalForces();

	// resolve forces of a single rigid body
	NlMath::Vector3D resolveTranslationalForces(int forceHandle);
	// resolve forces of a single rigid body
	NlMath::Vector3D resolveTranslationalForces(const ComponentRigidBody& rigidBody);



	//void addMomentum(const TranslationalMomentum& transForce);
	void addForce(const RotationalForce& transForce);
	void addMomentum(const RotationalMomentum& transForce);

	NlMath::Vector3D solveRotation();
	
};

} // NS
