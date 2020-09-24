#pragma once
#include "..\\Math\Vector.h"
#include "Components.h"

class ComponentTransform : public IComponent
{
public:
	NlMath::Vector3D _position;
	NlMath::Vector3D _rotation;
	NlMath::Vector3D _scale;

	ComponentTransform();
	~ComponentTransform();

	//read and write function for initialization from saved files
	void Read();
	void Write();
};