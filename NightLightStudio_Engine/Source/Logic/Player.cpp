#pragma once
#include "Player.h"

#define PLAYER_MOVE_MAG 1000.0f

/// <control mapping>
////////////////////////////////////////////////////////////////////////////////////////////
#define WALKLEFT SystemInput_ns::IKEY_A
#define WALKRIGHT SystemInput_ns::IKEY_D
#define WALKFRONT SystemInput_ns::IKEY_W
#define WALKBACK SystemInput_ns::IKEY_S
#define POSSESS SystemInput_ns::IMOUSE_RBUTTON
/// ////////////////////////////////////////////////////////////////////////////////////////
/// </control mapping>

Player::Player()
	:comCol(COLLIDERS::CAPSULE),comRigid(),comTrans(),_playerState(PLAYERSTATE::HUMAN),_prevPlayerState(PLAYERSTATE::HUMAN),_front(0), playerEntity(nullptr, -1)
{
}

void Player::init()
{
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk1", WALKFRONT, "WalkFront", SystemInput_ns::OnHold, [this]()
		{
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(currentCameraDirection.x,0,currentCameraDirection.z), PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk2", WALKLEFT, "WalkLeft", SystemInput_ns::OnHold, [this]()
		{
			
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3( -currentCameraDirection.x, 0,0), PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk3", WALKBACK, "WalkBack", SystemInput_ns::OnHold, [this]()
		{
			//negative direction
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(-currentCameraDirection.x, 0, -currentCameraDirection.z), PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk4", WALKRIGHT, "WalkRight", SystemInput_ns::OnHold, [this]()
		{
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(currentCameraDirection.x, 0, 0), PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Become Butterfly", WALKRIGHT, "butterfly", SystemInput_ns::OnPress, [this]()
		{
			changeState(PLAYERSTATE::BUTTERFLY);
		});
}

void Player::update()
{
	switch (_playerState)
	{
	case PLAYERSTATE::HUMAN:
	{
		/*TO DO*/ // take camera position as front
		_front = NlMath::Vec3(currentCameraDirection.x, 0, currentCameraDirection.z);

		break;
	}
		
	case PLAYERSTATE::BUTTERFLY:
	{
		_front = currentCameraDirection;
		
		break;
	}
		
	case PLAYERSTATE::POSSESSED:
		break;
	default:
		break;
	}
}

void Player::changeState(PLAYERSTATE state)
{
	_prevPlayerState = _playerState;
	_playerState = state;
	switch (_playerState)
	{
	case PLAYERSTATE::HUMAN:
	{
		if (_prevPlayerState == PLAYERSTATE::POSSESSED)
		{
			/*TO DO*/ 
			//set player's transform to spawn point transform
			
		}
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk1", WALKFRONT, "WalkFront", SystemInput_ns::OnHold, [this]()
			{
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, _front, PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk2", WALKLEFT, "WalkLeft", SystemInput_ns::OnHold, [this]()
			{
				glm::quat Quaternion(glm::radians(glm::vec3(0,-90,0)));
				glm::mat4 Rotate = glm::mat4_cast(Quaternion);
				NlMath::Matrix4x4 rotation;
				rotation = Rotate;
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, rotation * _front , PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk3", WALKBACK, "WalkBack", SystemInput_ns::OnHold, [this]()
			{
				//negative direction
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, -_front, PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk4", WALKRIGHT, "WalkRight", SystemInput_ns::OnHold, [this]()
			{
				glm::quat Quaternion(glm::radians(glm::vec3(0, 90, 0)));
				glm::mat4 Rotate = glm::mat4_cast(Quaternion);
				NlMath::Matrix4x4 rotation;
				rotation = Rotate;
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, rotation * _front, PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Become Butterfly", WALKRIGHT, "butterfly", SystemInput_ns::OnPress, [this]()
			{
				changeState(PLAYERSTATE::BUTTERFLY);
			});
		
		break;
	}

	case PLAYERSTATE::BUTTERFLY:
	{
		// player
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk4");
		break;
	}

	case PLAYERSTATE::POSSESSED:
	{
		
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk4");
		break;
	}
		
	default:
		break;
	}
}

bool Player::enterPossession()
{
	//check if the player is colliding with any object
	//check any object is possessable
	return true;
}
