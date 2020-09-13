/***********************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : Manifold.cpp
Author : Yap Junxian (100%)
DP email : junxian.yap@digipen.edu
Course : GAM200F19-A
Brief Description :

************************************************************************************/
#include "Manifold.h"
#include "Header.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Entity.h"
//#include "Components.h"

void CollisionResolver::SolveVelocity(Manifold& manifold)
{
	if (!manifold.mpBodyA || !manifold.mpBodyB)
		return;

	if (manifold.mpBodyA->GetStatic() && manifold.mpBodyB->GetStatic())
		return;

	Vec3 velo1 = manifold.mpBodyA->GetVelocity();
	Vec3 velo2 = manifold.mpBodyB->GetVelocity();

	float inverse_mass = manifold.mpBodyA->GetInvMass() + manifold.mpBodyB->GetInvMass();

	float JV = 0;//Vec3DotProduct(velo1 - velo2, manifold.normal);

	// Bounciness
	float e = 0;//std::min(manifold.mpBodyA->GetResituation(), manifold.mpBodyB->GetResituation());

	float lambda = -(1 + e) * JV / inverse_mass;

	Vec3 impulse_vector = lambda * manifold.normal;

	velo1 += manifold.mpBodyA->GetInvMass() * impulse_vector;
	velo2 -= manifold.mpBodyB->GetInvMass() * impulse_vector;

	manifold.mpBodyA->SetVelocity(velo1);
	manifold.mpBodyB->SetVelocity(velo2);
}

void CollisionResolver::SolvePosition(Manifold& manifold)
{
	const float k_slop = 0.0f; // Penetration allowance
	const float percent = 1.0f; // Penetration percentage to correct
	Vec3 correction = { 0.0f,0.0f, 0.0f };//(std::max(manifold.penetration - k_slop, 0.0f) / (manifold.mpBodyA->GetInvMass() + manifold.mpBodyB->GetInvMass())) * percent * manifold.normal;
	manifold.mpBodyA->GetOwner()->GetComponent<TransformComponent>().SetPosition(manifold.mpBodyA->GetOwner()->GetComponent<TransformComponent>().GetPosition()
		+/*=*/ manifold.mpBodyA->GetInvMass() * correction);
	manifold.mpBodyB->GetOwner()->GetComponent<TransformComponent>().SetPosition(manifold.mpBodyB->GetOwner()->GetComponent<TransformComponent>().GetPosition()
		-/*=*/ manifold.mpBodyB->GetInvMass() * correction);
	if (manifold.normal.y == 1)
	{
		Vec3 playerVecA = manifold.mpBodyA->GetOwner()->GetComponent<Rigidbody>().GetVelocity();
		playerVecA.y = 0;
		manifold.mpBodyA->SetVelocity(playerVecA);

		Vec3 playerVecB = manifold.mpBodyA->GetOwner()->GetComponent<Rigidbody>().GetVelocity();
		playerVecB.y = 0;
		manifold.mpBodyA->SetVelocity(playerVecB);
	}
}

void CollisionResolver::SolveCollisions()
{
	// player ounter
	int count = 0;

	// Sort the position of the objects
	for (Manifold& manifold : Allmanifold)
	{
		UNREFERENCED_PARAMETER(manifold);
		std::sort(Allmanifold.begin(), Allmanifold.end(), [](const Manifold& A, const Manifold& B) {
			float largestA, largestB;

			largestA = A.mpBodyA->GetOwner()->GetComponent<TransformComponent>().GetPosition().y
					 > A.mpBodyB->GetOwner()->GetComponent<TransformComponent>().GetPosition().y
					 ? A.mpBodyA->GetOwner()->GetComponent<TransformComponent>().GetPosition().y
					 : A.mpBodyB->GetOwner()->GetComponent<TransformComponent>().GetPosition().y;
					 
			largestB = B.mpBodyA->GetOwner()->GetComponent<TransformComponent>().GetPosition().y
					 > B.mpBodyB->GetOwner()->GetComponent<TransformComponent>().GetPosition().y
					 ? B.mpBodyA->GetOwner()->GetComponent<TransformComponent>().GetPosition().y
					 : B.mpBodyB->GetOwner()->GetComponent<TransformComponent>().GetPosition().y;

			return largestA < largestB;
		});
	}

	for (Manifold& manifold : Allmanifold)
	{
		SolveVelocity(manifold);
	}
	for (Manifold& manifold : Allmanifold)
	{
		SolvePosition(manifold);
		/*
		// Check player inside the vector list
		if (manifold.mpBodyA->GetOwner()->HasComponent<PlayerController>())
		{
			count++;
		}*/
	}

	// Set player ground if not found in the vector
	/*
	if (!count)
	{
		Entity* player = EntityManager::GetInstance()->GetEntityByName("Player");
		if(player!=nullptr)
			player->GetComponent<Rigidbody>().SetGround(false);
	}*/
}

void CollisionResolver::Exit()
{
	Allmanifold.clear();
}

void CollisionResolver::Unload()
{
	Exit();
	//DestroyInstance();
}

