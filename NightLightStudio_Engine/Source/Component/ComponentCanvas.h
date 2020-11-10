//#pragma once
//#include <string>
//#include "..\glm\glm.hpp"
//#include "..\\..\\ISerializable.h"
//#include "LocalString.h"
//
//enum UI_TYPE
//{
//	IMAGE,
//	ALL_UI
//};
//
//struct UIELement
//{
//	LocalString<256> _uiName;
//	UI_TYPE _type;
//
//	//Z will be the depth for drawing order
//	glm::vec3 _position;
//	glm::vec2 _size;
//	glm::vec3 _colour;
//
//	LocalString<256> _fileName;
//} UI;
//
//typedef class ComponentCanvas : public ISerializable//: public IComponent
//{
//public:
//	// Temporarily make them public for easy access
//
//	bool _isActive; // Temporarily set to true at beginning
//
//	/////////////////////////////////////////
//	/// Material without texture
//	/////////////////////////////////////////
//	LocalVector<UIELement, 32> _uiElements;
//
//	// Default constructor
//	ComponentCanvas();
//
//	// Destructor, destroy all OpenGL objects via graphics system
//	~ComponentCanvas();
//
//	//read and write function for initialization from saved files
//	void	Read(Value&);
//	Value	Write();
//	virtual Value& Write(Value& val) { return val; };
//	virtual ComponentCanvas* Clone()
//	{
//		ComponentCanvas* newcomp = new ComponentCanvas();
//		*newcomp = *this;
//		return newcomp;
//	}
//} CanvasComponent;