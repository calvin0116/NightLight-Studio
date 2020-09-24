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

	//getters and setters
	NlMath::Vector3D GetVelocity() const;
	NlMath::Vector3D SetVelocity(NlMath::Vector3D velocity);
	NlMath::Vector3D GetAcceleration() const;
	NlMath::Vector3D SetAcceleration(NlMath::Vector3D acceleration);
	NlMath::Vector3D GetPostion() const;
	NlMath::Vector3D GetPreviousPosition() const;
	NlMath::Vector3D SetPreviousPosition(NlMath::Vector3D prevPos);


	void Read();
	void Write();
};