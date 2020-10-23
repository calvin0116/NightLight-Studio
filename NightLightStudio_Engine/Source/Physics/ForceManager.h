#pragma once
#include <vector>

#include "../Math/Vector.h"



typedef struct TranslationalForce
{
	NlMath::Vector3D direction = 0;
	float magnitute = 0;
} Force;

typedef struct TranslationalMomentum
{
	NlMath::Vector3D direction = 0;
	float magnitute = 0;
} ;

typedef struct RotationalForce
{
	
} Torque;

typedef struct RotationalMomentum
{
	//L = MOR2
	float radius = 0;
	float mass = 0;
	NlMath::Vector3D angularMomentum;
} ;


class ForceManager
{
	std::vector<TranslationalForce> translationalForceList;
	std::vector<TranslationalMomentum> translationalMomentumList;
	std::vector<RotationalForce> rotationalForceList;
	std::vector<RotationalMomentum> rotationalMomentumList;


public:
	void addForce(const TranslationalForce& transForce);
	void addMomentum(const TranslationalMomentum& transForce);
	void addForce(const RotationalForce& transForce);
	void addMomentum(const RotationalMomentum& transForce);

	NlMath::Vector3D solveRotation();
	
};