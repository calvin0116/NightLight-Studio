#pragma once
#include "..\\Math\Vector.h"

#include "..\\..\ISerializable.h"

typedef struct ComponentRigidBody : public ISerializable //: public IComponent
{
	//velocity of the object
	NlMath::Vector3D velocity;
	//acceleration of the object
	NlMath::Vector3D acceleration;
	//previous position
	NlMath::Vector3D prevPos;
	//previous position
	NlMath::Vector3D nextPos;
	//determine if it is a static object, a static object will not receive update
	bool isStatic;
	float mass;
	ComponentRigidBody();
	~ComponentRigidBody();



	virtual void	Read(Value&) { };
	virtual Value	Write() { return Value(); };
	virtual Value& Write(Value& val) { return val; };

} RigidBody, RigidBodyComponent;