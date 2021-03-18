#include "ComponentAnimation.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Graphics/AnimationSystem.h"
#include "../Graphics/ModelManager.h"
#include "Components.h"
#include "ComponentManager.h"

ComponentAnimation::ComponentAnimation()
	: _isActive{ true },
	_controllerID{ -1 }
{
	strcpy_s(ser_name, "AnimationComponent");
}

ComponentAnimation::~ComponentAnimation()
{
	strcpy_s(ser_name, "AnimationComponent");
}

void ComponentAnimation::PlayAnimation(const std::string& newAnim, bool loop, double startFrame, double endFrame)
{
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->PlayAnimation(newAnim, this, loop, startFrame, endFrame);
}

void ComponentAnimation::PauseAnimation()
{
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play = false;
}

void ComponentAnimation::ResumeAnimation()
{
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play = true;
}

void ComponentAnimation::StopAnimation()
{
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play = false;
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_dt = 0.0f;
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim = "";
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_defaultAnim = "";
}

bool ComponentAnimation::IsFinished(const std::string& anim)
{
	//TracyMessage(anim.c_str(), anim.size());
	return !NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play &&
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim == anim;
}

bool ComponentAnimation::IsFinished()
{
	//TracyMessage(NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim.c_str(), NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim.size());
	return !NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play;
}

bool ComponentAnimation::GetPlaying() const
{
	return NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play;
}

//read and write function for initialization from saved files

void ComponentAnimation::SetAnimController(unsigned index)
{
	if (NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers.size() <= index)
	{
		//Doesn't exist
		return;
	}

	if (NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[index] == nullptr)
	{
		//Doesn't exist
		return;
	}

	_controllerID = index;
}

unsigned ComponentAnimation::GetAnimController()
{
	return _controllerID;
}

inline void ComponentAnimation::Read(Value& val)
{
	if (val.FindMember("isActive") == val.MemberEnd())
	{
		//std::cout << "No active data has been found" << std::endl;
		TracyMessageL("ComponentAnimation::Read: No active data has been found");
		SPEEDLOG("ComponentAnimation::Read: No active data has been found");
	}
	else
		_isActive = val["isActive"].GetBool();

	_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();

	if (val.FindMember("Play") == val.MemberEnd())
	{
		//std::cout << "No play data has been found" << std::endl;
		TracyMessageL("ComponentAnimation::Read: No play data has been found");
		SPEEDLOG("ComponentAnimation::Read: No play data has been found");
	}
	else
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play = val["Play"].GetBool();

	if (val.FindMember("Loop") == val.MemberEnd())
	{
		//std::cout << "No loop data has been found" << std::endl;
		TracyMessageL("ComponentAnimation::Read: No loop data has been found");
		SPEEDLOG("ComponentAnimation::Read: No loop data has been found");
	}
	else
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_loop = val["Loop"].GetBool();

	std::string currentAnim;
	if (val.FindMember("CurrentAnimation") == val.MemberEnd())
	{
		//std::cout << "No anim data has been found" << std::endl;
		TracyMessageL("ComponentAnimation::Read: No anim data has been found");
		SPEEDLOG("ComponentAnimation::Read: No anim data has been found");
	}
		
	else
		currentAnim = val["CurrentAnimation"].GetString();

	std::string defaultAnim;
	if (val.FindMember("DefaultAnimation") == val.MemberEnd())
	{
		//std::cout << "No anim data has been found" << std::endl;
		TracyMessageL("ComponentAnimation::Read: No anim data has been found");
		SPEEDLOG("ComponentAnimation::Read: No anim data has been found");
	}
	else
		defaultAnim = val["DefaultAnimation"].GetString();

	int i = 0;
	std::string animName = std::string("Animation").append(std::to_string(i));
	while (val.FindMember(animName.c_str()) != val.MemberEnd())
	{
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_allAnims.insert(val[animName.c_str()].GetString());
		++i;
		animName = std::string("Animation").append(std::to_string(i));
	}

	if (NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play)
	{
		if (!defaultAnim.empty())
		{
			PlayAnimation(defaultAnim);
		}
		else if (!currentAnim.empty())
		{
			PlayAnimation(currentAnim);
		}
	}

	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim = currentAnim;
	NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_defaultAnim = defaultAnim;
}

inline Value ComponentAnimation::Write()
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	NS_SERIALISER::ChangeData(&val, "Play", NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play);
	NS_SERIALISER::ChangeData(&val, "Loop", NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_loop);
	NS_SERIALISER::ChangeData(&val, "CurrentAnimation", rapidjson::StringRef(NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim.c_str()));
	NS_SERIALISER::ChangeData(&val, "DefaultAnimation", rapidjson::StringRef(NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_defaultAnim.c_str()));

	int i = 0;
	for(auto& name : NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_allAnims)
	{
		NS_SERIALISER::ChangeData(&val, std::string("Animation").append(std::to_string(i)), rapidjson::StringRef(name.c_str()));
		++i;
	}

	return val;
}
