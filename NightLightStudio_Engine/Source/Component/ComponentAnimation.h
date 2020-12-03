#pragma once
#include <string>
#include "..\\..\\ISerializable.h"

typedef class ComponentAnimation : public ISerializable//: public IComponent
{
public:
	int _controllerID; // value is -1 if no controller is assigned

	bool _isActive;

	// Default constructor
	ComponentAnimation();

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentAnimation();

	//C# Expose Functions
	void PlayAnimation(std::string newAnim, bool loop = true);
	void PauseAnimation();
	void ResumeAnimation();
	void StopAnimation();
	bool IsFinished();

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
