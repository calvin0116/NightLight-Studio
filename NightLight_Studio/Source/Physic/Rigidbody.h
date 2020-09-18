/***********************************************************************************
Brief Description :
		Rigidbody class that contains mostly physics related values 
************************************************************************************/
#ifndef  RIGID_BODY_H
#define RIGID_BODY_H

//#include "ECS.h"
//#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Vector.h"

//#include "DeltaTime.h"
//#include "Singleton.h"
//#include "Line.h"
//#include "GraphicsStructureManager.h"

class Rigidbody :public Component
{
	Vec3 drag;
	Vec3 position;
	Vec3 acceleration;
	Vec3 velocity;
	float mass;
	float invMass;
	float restitution;
	float gravity;
	float defaultGravity;

	Vec3 totalForce;
	Vec3 currentForce;
	Vec3 direction;

	bool isStatic;
	bool isGround = false;

	//bool freezePos[3];
	//bool freezeRot[3];

	// Velocity debug drawing/information
	//bool display_debug_drawing = false;
	//Line* debug_velocity_line = nullptr;

public:
	Vec3 tmpVelocity;
	bool cantMove = false;

	Rigidbody();
	Rigidbody(const Rigidbody& rhs);
	~Rigidbody();

	void Init() override;

	void AddForce(Vec3);
	void RemoveForce();
	Vec3 Calculation();
	void Initialize();

	void IntegrateVelocity();
	/*
	nlohmann::json Serialize() override;
	void Deserialize(nlohmann::json prefab) override;
	void DeserializeOverWrite(nlohmann::json level_obj) override;
	*/
	Rigidbody* Clone() override;
	// Getter and Setter function
	Vec3 GetDrag();
	void SetDrag(Vec3);

	float& GetMass();
	void SetMass(float);

	float GetInvMass();
	void SetInvMass(float);

	Vec3 GetForce();
	void SetForce(Vec3);

	bool& GetStatic();
	void SetStatic(bool);

	Vec3 GetVelocity();
	void SetVelocity(Vec3);

	Vec3 GetAcc();
	void SetAcc(Vec3);

	Vec3 GetDir();
	void SetDir(Vec3);

	float& GetDefault();

	/*bool GetPosBool(int);
	void SetPosBool(bool, int);

	bool GetRotBool(int);
	void SetRotBool(bool, int);*/

	bool GetGround();
	void SetGround(bool);

	float& GetResituation();
	void SetResituation(float);

	float& GetGravity();
	void SetGravity(float);

	Vec3 GetTempVelo();
	void SetPrevVelo(Vec3);

	/*
	bool GetDisplayDebugDrawingStatus() const;
	void SetDisplayDebugDrawingStatus(bool state);
	const Line* GetDebugVelocityLine() const;
	void UpdateDebugVelocityLine();
	*/
};
#endif // ! RIGID_BODY_H
//#include "Rigidbody.hpp"