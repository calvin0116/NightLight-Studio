#pragma once
#include <vector>

#include <map>

#include <ctime> // for colock

#include "../Math/Vector.h"

#include "..\Component\ComponentRigidBody.h"
#include "..\Component\ComponentManager.h"

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
	TranslationalForce() :
		direction(0.0f, 0.0f, 0.0f),
		magnitute(0.0f)
	{}
	TranslationalForce(NlMath::Vector3D dir, float mag) :
		direction(dir),
		magnitute(mag)
	{}
} Force;

///////////////////////////////////////////////////////////////////////////////////// TODO !!!
//typedef struct TimedTranslationalForce
//{
//	NlMath::Vector3D direction;
//	float magnitute;  //magnitude
//	clock_t activeUntil;
//	TimedTranslationalForce() :
//		direction(0.0f, 0.0f, 0.0f),
//		magnitute(0.0f),
//		activeUntil(clock())
//	{}
//	TimedTranslationalForce(NlMath::Vector3D dir, float mag) :
//		direction(dir),
//		magnitute(mag)
//	{}
//} TimedForce, Force_Timed, TranslationalForce_Timed;

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


	// a multimap of forcehandle to translation force
	std::multimap<int, TranslationalForce> translationalForceMap;
	// nxt tick
	std::multimap<int, TranslationalForce> translationalForceMap_next;
	// current key assignemnt
	int currentKeyIndex;

public:
	// add force to the rigidbody 
	void addForce(Entity ent, const TranslationalForce& transForce);
	// add force to the rigidbody 
	void addForce(Entity ent, NlMath::Vector3D direction, float magnitude);


	// add force to the rigidbody in the next tick
	void addForceToNextTick(Entity ent, NlMath::Vector3D direction, float magnitude);

	void updateTranslationalForces();
	void clearTranslationalForces();


	// resolve forces of a single rigid body
	NlMath::Vector3D resolveTranslationalForces(int entId);
	// resolve forces of a single rigid body
	NlMath::Vector3D resolveTranslationalForces(Entity ent);



	//void addMomentum(const TranslationalMomentum& transForce);
	void addForce(const RotationalForce& transForce);
	void addMomentum(const RotationalMomentum& transForce);

	NlMath::Vector3D solveRotation();
	
};

} // NS
