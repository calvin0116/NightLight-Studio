#pragma once
#include <string>
#include "..\glm\glm.hpp"
#include "..\\..\\ISerializable.h"
#include "LocalString.h"

enum UI_TYPE
{
	IMAGE,
	BUTTON,
	ALL_UI
};

enum CANVAS_TYPE
{
	SCREEN_SPACE,
	WORLD_SPACE
};

struct UI_Element
{
	LocalString<256> _uiName;
	UI_TYPE _type;

	//Z will be the depth for drawing order
	glm::vec3 _position;															// Expose
	glm::vec2 _size;																	// Expose
	glm::vec4 _colour = { 1.0f, 1.0f, 1.0f, 1.0f };		// Expose

	bool _isActive = true; // Expose
	bool _isAnimated = false; //Expose

	//Should be exposed?
	unsigned _row = 1;
	unsigned _column = 1;
	unsigned _totalFrame = 1;
	//Frame per second is given in unsigned like 60 fps
	//Which means animationRate should be 1s/60frames;
	unsigned _framesPerSecond = 1;

	//Not exposed Internal Usage
	unsigned _currentFrame = 0;
	float _animationRate = 1.0f;
	float _timePassed = 0.0f;

	//Should be exposed?
	bool _play = false;
	bool _loop = false;
	bool _autoPlay = false;

	LocalString<256> _fileName;
	unsigned _imageID;

	glm::mat4 GetModelMatrix();
	void AddTexture(std::string filename); // Expose
	void PlayAnimation(bool loop = false); // Expose
	void StopAnimation();

	//Expose this
	//bool OnClick();
	//Works only on screen space
	bool OnClick() const;
	bool OnHover() const;
	bool OnEnter() const;
	bool OnExit() const;
	bool OnRelease() const;

	bool operator<(const UI_Element& rhs);

private:
	bool _mouseEnter = false;
	bool _mouseExit = false;
	bool _mouseStay = false;
public:
	void CheckMouseCollision();
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
	//TODO 
	CANVAS_TYPE _canvasType;

	// Default constructor
	ComponentCanvas();

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentCanvas();

	//EXPOSE THIS
	void AddUI();								
	void RemoveUI(size_t index);
	void Sort();

	//To Expose
	UI_Element& FindUI(size_t index);
	UI_Element& FindUIByName(std::string name);

	//Extra function expose up to u
	UI_Element& operator[](size_t index);

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