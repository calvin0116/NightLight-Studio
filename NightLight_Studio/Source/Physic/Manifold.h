/***********************************************************************************
Breif Description
	A class to store all the collision to resolve them all at once
************************************************************************************/
#pragma once

#include <algorithm>
#include "Rigidbody.h"
#include "ColliderComponent.h"

struct Manifold
{
	void Init(Rigidbody* a, Rigidbody* b, Vec3 _normal)
	{
		mpBodyA = a;
		mpBodyB = b;
		normal = _normal;
	}

	Rigidbody* mpBodyA = nullptr;
	Rigidbody* mpBodyB = nullptr;
	Vec3 normal;

	float penetration;
};

class CollisionResolver //: public Singleton<CollisionResolver>, public ISystem
{
private:

public:
	std::vector<Manifold> Allmanifold;
	float highestheight;

	void SolveVelocity(Manifold& manifold);
	void SolvePosition(Manifold& manifold);
	void SolveCollisions();
	void Exit();

	void Unload();
	
};