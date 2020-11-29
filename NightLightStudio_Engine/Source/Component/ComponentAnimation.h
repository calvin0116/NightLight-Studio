#pragma once
#include <string>
#include "..\\..\\ISerializable.h"

typedef class ComponentAnimation : public ISerializable//: public IComponent
{
public:
	int _controllerID; // value is -1 if no controller is assigned
	int _modelID; // Filled during attachment

	bool _isActive;

	// Default constructor
	ComponentAnimation();

	// Parametrized constructor 
	ComponentAnimation(const int& meshID);

	void PlayAnimation(std::string anim, bool loop);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentAnimation();

	//read and write function for initialization from saved files
	void	Read(Value&);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentAnimation* Clone()
	{
		ComponentAnimation* newcomp = new ComponentAnimation();
		*newcomp = *this;
		return newcomp;
	}
} AnimationComponent;
