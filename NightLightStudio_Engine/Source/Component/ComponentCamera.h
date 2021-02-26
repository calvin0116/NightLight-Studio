#pragma once
#include "..\\..\\ISerializable.h"
#include "..\glm\glm.hpp"
#include "../Graphics/CameraSystem.h"
#include "../Graphics/Camera.h"

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
	NS_GRAPHICS::Camera _data;
	//// value is -1 if no camera is assigned
	//int _cameraID;

	//// View variables
	//// Camera Vectors & Position(Point)
	//glm::vec3 cameraPos;
	//glm::quat m_orientation;
	//glm::vec3 cameraTarget; // Not currently in use for multi camera

	//// Dictates how fast the camera will move
	//float _rotation_sensitivity = 1000.f;
	//float _drag_sensitivity = 200.f;
	//float _zoom_sensitivity = 50.f;

	//// Variables for camera rotation
	//// Given in radians
	//float yaw;	// x-axis rotation (Rotation about y axis vector)
	//float pitch;  // y-axis rotation (Rotation about x axis vector)
	//float roll;   // z-axis rotation (Rotation about z axis vector)

	////Camera Variables
	//CAMERA_FLAG _flag;
	//glm::vec3 _colour;
	//float _fov;

	////Clipping Plane
	//float _near;
	//float _far;

	////Viewport
	//float _x;
	//float _y;
	//float _w;
	//float _h;

	////Draw order
	//int _depth;

	// Default constructor
	ComponentCamera();

	//// Parametrized constructor 
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