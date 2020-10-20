#pragma once
#include <vector>

#include "../Math/Vector.h"



typedef struct DirectionalForce
{
	NlMath::Vector3D direction = 0;
	float magnitute = 0;
	


} Force;

typedef struct RotationalForce
{

} Torque;

struct ForceManager
{
	std::vector<DirectionalForce> ForceList;
	std::vector<RotationalForce> TorqueList;
};