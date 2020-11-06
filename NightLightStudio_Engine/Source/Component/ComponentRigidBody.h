#pragma once
#include "..\\Math\Vector.h"

#include "..\\..\ISerializable.h"

typedef struct ComponentRigidBody : public ISerializable //: public IComponent
{
	bool isActive;

	//sum of all forces acting on the object
	NlMath::Vector3D force;
	//acceleration of the object
	NlMath::Vector3D acceleration;
	//velocity of the object
	NlMath::Vector3D velocity;
	//previous position
	NlMath::Vector3D prevPos;
	//previous position
	NlMath::Vector3D nextPos;
	//determine if it is a static object, a static object will not receive update
	bool isStatic;
	//mass of the object
	float mass;
	//determine if the object is affected by gravity
	bool isGravity;
	//gravity accleration of the object
	NlMath::Vector3D gravity;

	// handle to the force handled by force manager
	int forceHandle;

	ComponentRigidBody();
	~ComponentRigidBody();

	void	Read(Value& val);
	Value	Write();
	Value& Write(Value& val) { return val; };
	virtual ComponentRigidBody* Clone()
	{
		ComponentRigidBody* newcomp = new ComponentRigidBody();
		*newcomp = *this;
		return newcomp;
	}

} RigidBody, RigidBodyComponent;