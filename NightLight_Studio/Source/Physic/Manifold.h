/***********************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : Manifold.h
Author : Yap Junxian (100%)
DP email : junxian.yap@digipen.edu
Course : GAM200F19-A
Brief Description :

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