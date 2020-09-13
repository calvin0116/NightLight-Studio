  #pragma once
#include "Header.h"
#include "MySystem.h"
#include "RigidBody.h"
#include "Singleton.h"

class PhysicManager : public MySystem, public Singleton<PhysicManager>
{
private:
	//For singleton to access constructor / destructor
	friend Singleton<PhysicManager>;
	//List of all rigidbody created
	vector<Rigidbody> rb_vec;

	//Pointer to all active rigidbody
	vector<Rigidbody*> active_rb;

protected:
	PhysicManager()
	:	rb_vec(10),
		active_rb(10)
	{

	}

	~PhysicManager()
	{
		rb_vec.clear();
	}

public:
	void EarlyLoad() {
		//If there is a need for settings in Physics
		//E.g Overall bouncyness? / Update frequency?
		//Only used if you are loading for json

		//====Good to have (pre-made object) =====//
		int num_of_initial_rb = 10;		//for pooling purposes / pre-made rigid body for game use
		float gravity = 0.1;			//to be a variable in json or predefined

		Rigidbody basic_rb;
		basic_rb.SetGravity(gravity);

		//Initial number of rigid body to initialised
		//basic_rb is passed in for initial rigid body value
		rb_vec.resize(num_of_initial_rb , basic_rb);
	};

	void EarlyInit()
	{
		
	};

	void LateInit()
	{
		std::cout << "Late init physcis" << std::endl;
	}

	void Update() {
		
	};

	void Exit() {
		rb_vec.clear();
	};



	//============ Helper function===================//
	//Easy access to add to list
	void Add(Rigidbody _rb)
	{
		rb_vec.push_back(_rb);
	}

	void Add(vector <Rigidbody>& _rb_vec)
	{
		for(Rigidbody& _rb: _rb_vec)
			rb_vec.push_back(_rb);
	}
};