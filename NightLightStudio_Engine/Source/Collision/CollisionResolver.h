#pragma once
#include "../Math/Vector.h"


struct CollisionEvent
{
	NlMath::Vector3D collisionPoint;

};

void CollisionResolver();