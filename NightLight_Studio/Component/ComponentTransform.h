#pragma once
#include "..\Source\Vector.h"

struct ComponentTransform
{
	NlMath::Vector3D _position;
	NlMath::Vector3D _rotation;
	NlMath::Vector3D _scale;

	ComponentTransform();
	~ComponentTransform();

	//To add other stuff
};