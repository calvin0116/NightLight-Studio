#pragma once
#include "../Math/Vector.h"
#include "../Math/Matrix4x4.h"
#include "../Component/ComponentRigidBody.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Physics/SystemPhysics.h"
#include "../Input/SystemInput.h"
#include "../../glm/gtx/euler_angles.hpp"

enum class PLAYERSTATE
{
	HUMAN = 0,
	BUTTERFLY,
	POSSESSED
};
class Player
{
	Entity playerEntity;
	NlMath::Vector3D currentCameraDirection;
	
	//hardcoded for testing
	ComponentCollider comCol;
	ComponentRigidBody comRigid;
	ComponentTransform comTrans;

	PLAYERSTATE _prevPlayerState;
	PLAYERSTATE _playerState;
	NlMath::Vector3D _front;
	
public:
	Player();
	void init();
	void update();
	void changeState(PLAYERSTATE state);
	bool enterPossession();
};