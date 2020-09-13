/***********************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : RigidBody.cpp
Author : Yap Junxian (100%)
DP email : junxian.yap@digipen.edu
Course : GAM200F19-A
Brief Description :

************************************************************************************/
#include "Rigidbody.h"
#include "TransformComponent.h"
#include "Entity.h"
//#include "PlayerController.h"

Rigidbody::Rigidbody()
{
	totalForce = { 0,0,0 };
	velocity = { 0,0,0 };
	SetComponentName("Rigidbody");

	isStatic = true;
}


Rigidbody::Rigidbody(const Rigidbody& rhs)
	: Component(rhs),
	totalForce(rhs.totalForce), 
	velocity(rhs.velocity),
	isStatic(rhs.isStatic),
	restitution(rhs.restitution),
	mass(rhs.mass),
	gravity(rhs.gravity)
	
{
	SetComponentName("Rigidbody");

	Init();
}


Rigidbody::~Rigidbody()
{
}


void Rigidbody::Init()
{
	/*
	debug_velocity_line = GraphicsStructureManager::GetInstance()->CreateLine();
	debug_velocity_line->SetStartPoint(0, 0);
	debug_velocity_line->SetEndPoint(1, 0);
	debug_velocity_line->SetVerticesColour(0, 0, 1);
	*/
}


void Rigidbody::AddForce(Vec3 force)
{
	totalForce += force;
}

void Rigidbody::RemoveForce()
{
	totalForce = { 0,0,0 };
}

Vec3 Rigidbody::Calculation() // Integrate Position
{
	position = GetOwner()->GetComponent<TransformComponent>().GetPosition();
	
	if (isStatic || mass == 0)
	{
		return position;
	}

	const float dt = 0.13f;//= DeltaTime::GetInstance()->GetFixedDeltaTime();
	const float damping_factor = 0.9f; // MUST be 0 to 1

	acceleration = { 0, GetGravity() * mass, 0 };

	Vec3 new_acc = { 0, 0, 0 };
	new_acc = totalForce * invMass + acceleration;

	// integrate
	velocity += new_acc * dt;
	// Apply some damping to the velocity.
	velocity *= std::pow(1.0f - damping_factor, dt);
	
	if (cantMove)
	{
		velocity.x = 0;
	}
	position += velocity * dt;

	// Reset the force
	totalForce = { 0,0,0 }; // TODO: Make a new func: ClearForces()

	return position;
}

void Rigidbody::Initialize()
{
	if (mass > 0.0f)
	{
		isStatic = false;
		invMass = 1.0f / mass;
	}
	else
	{
		isStatic = true;
		invMass = 0.0f;
	}
}

/*
nlohmann::json Rigidbody::Serialize()
{
	nlohmann::json sData;

	sData[GetComponentName()]["Mass"] = mass;
	sData[GetComponentName()]["Static"] = isStatic;
	sData[GetComponentName()]["Restitution"] = restitution;
	sData[GetComponentName()]["Gravity"] = gravity;
	return sData;
}


void Rigidbody::Deserialize(nlohmann::json prefab)
{
	nlohmann::json& obj = prefab["Rigidbody"];

	if (CheckJsonVarExist(obj, "Mass"))
		SetMass(obj["Mass"]);

	if (CheckJsonVarExist(obj, "Static"))
		SetStatic(obj["Static"]);

	if (CheckJsonVarExist(obj, "Restitution"))
		SetResituation(obj["Restitution"]);

	if (CheckJsonVarExist(obj, "Gravity"))
		SetGravity(obj["Gravity"]);
}

void Rigidbody::DeserializeOverWrite(nlohmann::json level_obj)
{
	nlohmann::json& obj = level_obj["Rigidbody"];

	if (CheckJsonVarExist(obj, "Mass"))
		SetMass(obj["Mass"]);

	if (CheckJsonVarExist(obj, "Static"))
		SetStatic(obj["Static"]);

	if (CheckJsonVarExist(obj, "Restitution"))
		SetResituation(obj["Restitution"]);

	if (CheckJsonVarExist(obj, "Gravity"))
		SetGravity(obj["Gravity"]);
}
*/

Rigidbody* Rigidbody::Clone()
{
	return new Rigidbody(*this);
}


void Rigidbody::IntegrateVelocity()
{
	if (isStatic || mass == 0)
	{
		velocity = { 0,0,0 };
		return;
	}

	const float dt = 0.14f;//DeltaTime::GetInstance()->GetFixedDeltaTime();
	const float damping_factor = 0.9f; // MUST be 0 to 1

	acceleration = { 0, GetGravity() * mass, 0 };

	// Apply some damping to the velocity.
	velocity *= std::pow(1.0f - damping_factor, dt);

	Vec3 new_acc = { 0, 0, 0 };
	new_acc = totalForce * invMass + acceleration;

	// integrate
	velocity += new_acc * dt;
	if (cantMove)
	{
		velocity.x = 0;
	}
}



 Vec3 Rigidbody::GetDrag()
{
	return drag;
}

 void Rigidbody::SetDrag(Vec3 _drag)
{
	drag = _drag;
}

 float& Rigidbody::GetMass()
{
	Initialize();
	return mass;
}

 void Rigidbody::SetMass(float _mass)
{
	mass = _mass;
	Initialize();
}

 float Rigidbody::GetInvMass()
{
	invMass = 0;
	if (mass > 0)
		invMass = 1 / mass;
	return invMass;
}

 void Rigidbody::SetInvMass(float _invMass)
{
	invMass = _invMass;
}

 Vec3 Rigidbody::GetForce()
{
	return totalForce;
}

 void Rigidbody::SetForce(Vec3 _force)
{
	totalForce = _force;
}

 bool& Rigidbody::GetStatic()
{
	return isStatic;
}

 void Rigidbody::SetStatic(bool _static)
{
	isStatic = _static;
}

 Vec3 Rigidbody::GetVelocity()
{
	return velocity;
}

 void Rigidbody::SetVelocity(Vec3 _velocity)
{
	velocity = _velocity;
}

 Vec3 Rigidbody::GetAcc()
{
	return acceleration;
}

 void Rigidbody::SetAcc(Vec3 _acc)
{
	acceleration = _acc;
}

 Vec3 Rigidbody::GetDir()
{
	return currentForce;
}

 void Rigidbody::SetDir(Vec3 _force)
{
	currentForce = _force;
}

 float& Rigidbody::GetDefault()
 {
	 return defaultGravity;
 }

 bool Rigidbody::GetGround()
{
	return isGround;
}

 void Rigidbody::SetGround(bool _boolean)
{
	isGround = _boolean;
}

 float& Rigidbody::GetResituation()
{
	return restitution;
}

 void Rigidbody::SetResituation(float _res)
{
	restitution = _res;
}

 float& Rigidbody::GetGravity()
 {
	 /*
	 if (GetOwner()->HasComponent<PlayerController>() && GetOwner()->GetComponent<PlayerController>().isGliding == false
		  && GetOwner()->GetComponent<PlayerController>().isJumping == false)
	 {
		 defaultGravity = gravity;
	 }*/
	 return gravity;
 }

 void Rigidbody::SetGravity(float _gravity)
 {
	 gravity = _gravity;
	
 }

 Vec3 Rigidbody::GetTempVelo()
{
	return tmpVelocity;
}

 void Rigidbody::SetPrevVelo(Vec3 _prev)
{
	tmpVelocity = _prev;
}

 /*
 bool Rigidbody::GetDisplayDebugDrawingStatus() const
 {
	 return display_debug_drawing;
 }


 void Rigidbody::SetDisplayDebugDrawingStatus(bool state)
 {
	 display_debug_drawing = state;
 }


const Line* Rigidbody::GetDebugVelocityLine() const
 {
	 return debug_velocity_line;
 }


void Rigidbody::UpdateDebugVelocityLine()
{
	//if (debug_velocity_line)
	//	debug_velocity_line->SetEndPoint(velocity.x * 0.25f, velocity.y * 0.25f);
}*/

