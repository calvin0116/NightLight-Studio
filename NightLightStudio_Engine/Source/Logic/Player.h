#pragma once
#include "../Math/Vector.h"
#include "../Math/Matrix4x4.h"
#include "../Component/ComponentRigidBody.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Physics/SystemPhysics.h"
#include "../Input/SystemInput.h"
#include "../../glm/gtx/euler_angles.hpp"
#include "CScripts/IScript.h"
#include "CameraScript.h"
#include "../Graphics/CameraSystem.h"




enum class PLAYERSTATE
{
	HUMAN = 0,
	BUTTERFLY,
	POSSESSED
};

class Player : public IScript
{
	Entity playerEntity;
	AllScripts::CameraScript*camera;

	NlMath::Vector3D currentCameraDirection;
	
	//keep a copy of the component
	ComponentCollider* comCol;
	ComponentRigidBody* comRigid;
	ComponentTransform* comTrans;
	ComponentTransform spawnPoint;

	PLAYERSTATE _prevPlayerState;
	PLAYERSTATE _playerState;
	NlMath::Vector3D _front;
	
public:
	Player();
	void Init() override;
	void Update() override;
	void Exit() override;
	PLAYERSTATE getState();
	void changeState(PLAYERSTATE state);
	void setSpawnPoint(ComponentTransform& _spawnPoint);
	bool enterPossession();
	virtual void OnCollisionEnter(Entity other) override;
};