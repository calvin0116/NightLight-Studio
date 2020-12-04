#pragma once
#include <string>
#include "..\glm\glm.hpp"
#include "..\\..\\ISerializable.h"
#include "LocalString.h"

enum UI_TYPE
{
	IMAGE,
	ALL_UI
};

struct UI_Element
{
	LocalString<256> _uiName;
	UI_TYPE _type;

	//Z will be the depth for drawing order
	glm::vec3 _position;
	glm::vec2 _size;
	glm::vec4 _colour = { 1.0f, 1.0f, 1.0f, 1.0f };

	bool _isActive = true;

	LocalString<256> _fileName;
	unsigned _imageID;

	glm::mat4 GetModelMatrix();
	void AddTexture(std::string filename);

	//Expose this
	bool OnClick();

	bool operator<(const UI_Element& rhs);
};

typedef class ComponentCanvas : public ISerializable//: public IComponent
{
public:
	// Temporarily make them public for easy access

	bool _isActive; // Temporarily set to true at beginning

	/////////////////////////////////////////
	/// Material without texture
	/////////////////////////////////////////
	LocalVector<UI_Element, 32> _uiElements;

	// Default constructor
	ComponentCanvas();

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentCanvas();

	void AddUI();
	void RemoveUI(size_t index);
	void Sort();

	//To Expose
	UI_Element& FindUI(size_t index);
	UI_Element& FindUIByName(LocalString<256> name);

	//read and write function for initialization from saved files
	void	Read(Value&);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentCanvas* Clone()
	{
		ComponentCanvas* newcomp = new ComponentCanvas();
		*newcomp = *this;
		return newcomp;
	}
} CanvasComponent;