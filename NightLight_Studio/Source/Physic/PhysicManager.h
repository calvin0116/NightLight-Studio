/***************************************************************************
Brief Description :
		A Physic and pseudo force manager
**************************************************************************/

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
		cout << "Physic Engine EarlyLoad has started" << endl;
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

		cout << "Physic Engine EarlyLoad has ended" << endl;
	};
	/*
	void EarlyInit()
	{
		cout << "Physic Engine EarlyInit has started" << endl;

		cout << "Physic Engine EarlyInit has ended" << endl;
	};

	void LateInit()
	{
		cout << "Physic Engine LateInit has started" << endl;

		cout << "Physic Engine LateInit has ended" << endl;
	}

	void Update() {
		cout << "Physic Engine Update has started" << endl;

		cout << "Physic Engine Update has ended" << endl;
	};

	void Exit() {
		cout << "Physic Engine Exit has started" << endl;
		rb_vec.clear();
		cout << "Physic Engine Exit has ended" << endl;
	};
	*/

	//Physic requires fixedupdate as fixed update will always be run no matter the framerate
	void FixedUpdate()
	{
		

	}

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