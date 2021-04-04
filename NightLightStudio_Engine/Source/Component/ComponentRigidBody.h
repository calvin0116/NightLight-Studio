#pragma once
#include "..\\Math\Vector.h"
#include "..\\Math\Matrix4x4.h"
#include "..\\..\ISerializable.h"

typedef struct ComponentRigidBody : public ISerializable //: public IComponent
{
	bool isActive;      // Expose

	//sum of all forces acting on the object
	NlMath::Vector3D force;         // Expose
	//sum of all angular forces acting on the object
	NlMath::Vector3D angularForce;
	//acceleration of the object
	NlMath::Vector3D acceleration;  // Expose
	//angular acceleration of the object
	NlMath::Vector3D angularAcceleration;
	//velocity of the object
	NlMath::Vector3D velocity;      // Expose
	//angular velocity of the object
	NlMath::Vector3D angularVelocity;
	//inertia 
	NlMath::Matrix4x4 inertia;
	//previous position
	NlMath::Vector3D prevprevprevPos;
	//previous position
	NlMath::Vector3D prevprevPos;
	//previous position
	NlMath::Vector3D prevPos;
	//previous position
	NlMath::Vector3D nextPos;
	//determine if it is a static object, a static object will not receive update
	bool isStatic;      // Expose
	//mass of the object
	float mass;         // Expose
	//determine if the object is affected by gravity
	bool isGravity;       // Expose
	//gravity accleration of the object
	NlMath::Vector3D gravity;
	// friction coefficient of the object
	float friction;       // Expose

	// handle to the force handled by force manager
	int forceHandle;

	// sleep physics
	//bool isSleepX;
	//bool isSleepY;
	//bool isSleepZ;

	ComponentRigidBody();
	~ComponentRigidBody();

	void	Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER);
	Value	Write();
	Value& Write(Value& val) { return val; };
	virtual ComponentRigidBody* Clone()
	{
		ComponentRigidBody* newcomp = new ComponentRigidBody();
		*newcomp = *this;
		return newcomp;
	}

} RigidBody, RigidBodyComponent;