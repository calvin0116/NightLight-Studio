#include "ComponentRigidBody.h"

#include "Components.h"
//#include "ComponentTransform.h"


constexpr float GRAVITY_DEF = 980.0f;

ComponentRigidBody::ComponentRigidBody() :
	isActive(true),
	force(0.0f, 0.0f, 0.0f),
	angularForce(0.0f, 0.0f, 0.0f),
	acceleration(0.0f, 0.0f, 0.0f),
	angularAcceleration(0.0f, 0.0f, 0.0f),
	velocity(0.0f, 0.0f, 0.0f),
	angularVelocity(0.0f, 0.0f, 0.0f),
	prevPos(0.0f, 0.0f, 0.0f),
	nextPos(0.0f, 0.0f, 0.0f),
	isStatic(true),
	mass(0.0f),
	isGravity(false),
	gravity(0.0f, -GRAVITY_DEF, 0.0f),
	forceHandle(-1), //default -1 to ininit
	friction(0.01f)/*,
	isSleepX(false),
	isSleepY(false),
	isSleepZ(false)*/
{
	strcpy_s(ser_name,"RigidBody");
}

ComponentRigidBody::~ComponentRigidBody()
{
}

inline void ComponentRigidBody::Read(Value& val) 
{ 
	if (val.FindMember("isStatic") == val.MemberEnd())
	{
		TracyMessageL("ComponentRigidBody::Read: No Force data has been found");
		//std::cout << "No Force data has been found" << std::endl;
	}
	else
	{
		isStatic = val["isStatic"].GetBool();
	}

	if (val.FindMember("isGravity") == val.MemberEnd())
	{
		TracyMessageL("ComponentRigidBody::Read: No gravity data has been found");
		//std::cout << "No gravity data has been found" << std::endl;
	}
	else
	{
		isGravity = val["isGravity"].GetBool();
	}

	if (val.FindMember("mass") == val.MemberEnd())
	{
		TracyMessageL("ComponentRigidBody::Read: No mass data has been found");
		//std::cout << "No mass data has been found" << std::endl;
	}
	else
	{
		mass = val["mass"].GetFloat();
	}
	
	if (val.FindMember("friction") == val.MemberEnd())
	{
		TracyMessageL("ComponentRigidBody::Read: No mass friction has been found");
		//std::cout << "No mass friction has been found" << std::endl;
	}
	else
	{
		friction = val["friction"].GetFloat();
	}

	if (val.FindMember("Force") == val.MemberEnd())
	{
		TracyMessageL("ComponentRigidBody::Read: No Force data has been found");
		//std::cout << "No Force data has been found" << std::endl;
	}
	else
	{
		auto f = val["Force"].GetArray();

		force.x = f[0].GetFloat();
		force.y = f[1].GetFloat();
		force.z = f[2].GetFloat();
	}

	if (val.FindMember("Accelaration") == val.MemberEnd())
	{
		TracyMessageL("ComponentRigidBody::Read: No Force data has been found");
		//std::cout << "No Force data has been found" << std::endl;
	}
	else
	{
		auto ac = val["Accelaration"].GetArray();

		acceleration.x = ac[0].GetFloat();
		acceleration.y = ac[1].GetFloat();
		acceleration.z = ac[2].GetFloat();
	}
}

inline Value ComponentRigidBody::Write() 
{
	Value val(rapidjson::kObjectType);
	
	NS_SERIALISER::ChangeData(&val, "isStatic", isStatic);		//Bool
	NS_SERIALISER::ChangeData(&val, "isGravity", isGravity);	//Bool


	NS_SERIALISER::ChangeData(&val, "mass", mass);	//Float
	NS_SERIALISER::ChangeData(&val, "friction", friction);	//Float

	Value force_val(rapidjson::kArrayType);
	force_val.PushBack(force.x, global_alloc);
	force_val.PushBack(force.y, global_alloc);
	force_val.PushBack(force.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Force", force_val);		//Array

	Value accel_val(rapidjson::kArrayType);
	accel_val.PushBack(acceleration.x, global_alloc);
	accel_val.PushBack(acceleration.y, global_alloc);
	accel_val.PushBack(acceleration.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Accelaration", accel_val);	//Array
	

	return val;
}

