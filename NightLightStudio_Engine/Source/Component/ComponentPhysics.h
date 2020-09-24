#pragma once
#include "..\\Math\Vector.h"
#include "Components.h"
#include "ComponentTransform.h"

class ComponentPhysics : public IComponent
{
	//determine if it is a static object, a static object will not receive update
	bool _static;
	float _mass;
	//velocity of the object
	NlMath::Vector3D _velocity;
	//acceleration of the object
	NlMath::Vector3D _acceleration;
	//current position
	NlMath::Vector3D _pos;
	//previous position
	NlMath::Vector3D _prevPos;

	
public:
	ComponentPhysics();
	~ComponentPhysics();

	void Read();
	void Write();
};