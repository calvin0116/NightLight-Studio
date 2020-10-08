#pragma once
#include "..\\Math\Vector.h"
#include "..\\Math\Matrix4x4.h"
#include "Components.h"
#include "..\glm\glm.hpp"

class ComponentTransform : public IComponent
{
public:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;

	glm::mat4x4 _model;

	ComponentTransform();
	~ComponentTransform();

	//read and write function for initialization from saved files
	void Read();
	void Write();
};