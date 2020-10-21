#pragma once
#include "..\\Math\Vector.h"
#include "..\\Math\Matrix4x4.h"
#include "Components.h"
#include "..\glm\glm.hpp"

class ComponentTransform : public IComponent
{
public:
	//for collision check
	NlMath::Vector3D _nextPos;

	glm::vec3 _position;
	glm::vec3 _rotation; // Euler angles x, y, z
	glm::vec3 _scale;

	ComponentTransform();
	~ComponentTransform();

	glm::mat4 GetModelMatrix();

	//read and write function for initialization from saved files
	void Read();
	void Write();
};