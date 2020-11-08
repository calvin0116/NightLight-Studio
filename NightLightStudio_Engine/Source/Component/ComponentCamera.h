#pragma once
#include "..\\..\\ISerializable.h"
#include "..\glm\glm.hpp"

enum CAMERA_FLAG
{
	SKYBOX,
	COLOUR,
	ALL_FLAG
};

typedef class ComponentCamera : public ISerializable //: public IComponent
{
public:
	// Temporarily make them public for easy access

	bool _isActive; // Temporarily set to true at beginning

	// value is -1 if no light is assigned
	int _cameraID;

	//Camera Variables
	CAMERA_FLAG _flag;
	glm::vec3 _colour;
	float _fov;

	//Clipping Plane
	float _near;
	float _far;

	//Viewport
	float _x;
	float _y;
	float _w;
	float _h;

	//Draw order
	float _depth;

	// Default constructor
	ComponentCamera();

	// Parametrized constructor 
	ComponentCamera(const int& cameraID);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentCamera();

	/////////// GETTERS/SETTERS ////////////

	////////////////////////////////////////

	//read and write function for initialization from saved files
	void Read(Value& val);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentCamera* Clone()
	{
		ComponentCamera* newcomp = new ComponentCamera();
		*newcomp = *this;
		return newcomp;
	}
} CameraComponent;