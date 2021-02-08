#pragma once
#include "..\\..\\ISerializable.h"
#include "..\glm\glm.hpp"
#include "../Graphics/CameraSystem.h"
#include "..\Math\Vector.h"

enum CAMERA_FLAG
{
	SKYBOX,
	COLOUR,
	ALL_FLAG
};

// For gameplay only
class ComponentCamera : public ISerializable //: public IComponent
{
public:
	//int _cameraID; // value is -1 if no camera is assigned

	// View variables
	// Camera Vectors & Position(Point)
  bool _isActive; // Camera component is active.
  NlMath::Vec3      _Offset; // X, Y, Dist offset to tgt.
  //NlMath::Vec3      _color;
	glm::quat         _orientation;
  bool              _lerp;
  bool              _followTgt;
  LocalString<256>  _tgt;
	//glm::vec3 cameraTarget; // Not currently in use for multi camera

	// Dictates how fast the camera will move
	float _rotation_sensitivity;
	float _drag_sensitivity;
	float _zoom_sensitivity;

	// Variables for camera rotation
	// Given in radians
	float _yaw;	// x-axis rotation (Rotation about y axis vector)
	float _pitch;  // y-axis rotation (Rotation about x axis vector)
	float _roll;   // z-axis rotation (Rotation about z axis vector)

	//Camera Variables
	//CAMERA_FLAG _flag;
	//glm::vec3 _colour;
	float _fov;

	//Clipping Plane
	float _near;
	float _far;

	//Viewport
	//float _x;
	//float _y;
	//float _w;
	//float _h;

	//Draw order
	//int _depth;

	// Default constructor
	ComponentCamera();

	// Parametrized constructor
	//ComponentCamera(const int& cameraID);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentCamera();

	/////////// GETTERS/SETTERS ////////////

	// Set as current scene camera
	// Editor camera will still take precedence if editor is active
	// utilizes _isActive
	// Note that camera will be created here if no existing camera id is connected
	//void SetCurrentCamera();

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