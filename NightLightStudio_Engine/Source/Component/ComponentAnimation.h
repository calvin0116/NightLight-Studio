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
	void PlayAnimation(const std::string& newAnim, bool loop = true, double startFrame = -1.0, double endFrame = -1.0, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER);
	void PauseAnimation();
	void ResumeAnimation();
	void StopAnimation();
	bool IsFinished(const std::string& anim);
	bool IsFinished();

	void SetAnimController(unsigned index);
	unsigned GetAnimController();

	//read and write function for initialization from saved files
	void	Read(Value&, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentAnimation* Clone()
	{
		ComponentAnimation* newcomp = new ComponentAnimation();
		*newcomp = *this;
		return newcomp;
	}
} AnimationComponent;
