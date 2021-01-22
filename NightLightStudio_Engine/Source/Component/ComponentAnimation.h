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
	void PlayAnimation(const std::string& newAnim, bool loop = true, double startFrame = -1.0, double endFrame = -1.0);
	void PauseAnimation();
	void ResumeAnimation();
	void StopAnimation();
	bool IsFinished(const std::string& anim);
	bool IsFinished();

	//Alex expose this (I try to think of a way to swap animation of models mid game without 
	//it crashing so this is what i came up with)
	/////////////////////////////////////////////////////////
	/// //With human model with animation loaded
	/// unsigned humanAnim = GetAnimController();
	/// //Change model to moth that has animation
	/// unsigned mothAnim = GetAnimController();
	/// //Whenever form change just use
	/// SetAnimController(humanAnim); //Human mode
	/// SetAnimController(mothAnim); //Moth mode
	/////////////////////////////////////////////////////////
	void SetAnimController(unsigned index);
	unsigned GetAnimController();

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
