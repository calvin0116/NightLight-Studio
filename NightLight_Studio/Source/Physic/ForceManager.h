#pragma once
#include "Singleton.h"
#include "MySystem.h"

#include "Rigidbody.h"

class ForceManager : public MyManager, public Singleton<ForceManager>
{
private:
	friend Singleton <ForceManager>;
	//Data that needs to be collected
	//Pointer to a vector of collider component
	std::vector<Rigidbody*>* rigidbody_list;	//Can be changed according to needs



protected:
	ForceManager() {};
	~ForceManager() {};
};

