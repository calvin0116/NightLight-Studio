#pragma once
#include "Component.h"
class TriggerComponent :
	public Component
{
public:
	TriggerComponent();
	~TriggerComponent();

	virtual void OnTriggerEnter() {};
	virtual void OnTriggerExit() {};
	virtual void OnTriggerStay() {};
};

