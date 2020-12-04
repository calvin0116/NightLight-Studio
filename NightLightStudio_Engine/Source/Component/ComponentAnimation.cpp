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
}

bool ComponentAnimation::IsFinished(const std::string& anim)
{
	return !NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play &&
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim == anim;
}

//read and write function for initialization from saved files

inline void ComponentAnimation::Read(Value& val)
{
	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		_isActive = val["isActive"].GetBool();

	_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();

	if (val.FindMember("Play") == val.MemberEnd())
		std::cout << "No play data has been found" << std::endl;
	else
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play = val["Play"].GetBool();

	if (val.FindMember("Loop") == val.MemberEnd())
		std::cout << "No loop data has been found" << std::endl;
	else
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_loop = val["Loop"].GetBool();

	if (val.FindMember("CurrentAnimation") == val.MemberEnd())
		std::cout << "No anim data has been found" << std::endl;
	else
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim = val["CurrentAnimation"].GetString();

	int i = 0;
	std::string animName = std::string("Animation").append(std::to_string(i));
	while (val.FindMember(animName.c_str()) != val.MemberEnd())
	{
		NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_allAnims.insert(val[animName.c_str()].GetString());
		++i;
		animName = std::string("Animation").append(std::to_string(i));
	}
}

inline Value ComponentAnimation::Write()
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	NS_SERIALISER::ChangeData(&val, "Play", NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_play);
	NS_SERIALISER::ChangeData(&val, "Loop", NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_loop);
	NS_SERIALISER::ChangeData(&val, "CurrentAnimation", rapidjson::StringRef(NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_currAnim.c_str()));

	int i = 0;
	for(auto& name : NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[_controllerID]->_allAnims)
	{
		NS_SERIALISER::ChangeData(&val, std::string("Animation").append(std::to_string(i)), rapidjson::StringRef(name.c_str()));
		++i;
	}

	return val;
}
