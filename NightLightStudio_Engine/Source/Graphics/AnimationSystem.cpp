#include "AnimationSystem.h"
#include "GraphicsSystem.h"

NS_GRAPHICS::AnimationSystem::AnimationSystem()
{
	_usedStatus.resize(INITIAL_MAX_COUNT, false);
	_animControllers.resize(INITIAL_MAX_COUNT, nullptr);
}

NS_GRAPHICS::AnimationSystem::~AnimationSystem()
{
}

void NS_GRAPHICS::AnimationSystem::Update()
{
	for (auto& anim : _animControllers)
	{
		if (anim)
		{
			anim->Update(DELTA_T->real_dt);// * DT_SCALE);
		}
	}
}

void NS_GRAPHICS::AnimationSystem::Free()
{
	for (auto& anim : _animControllers)
	{
		delete anim;
	}
}

void NS_GRAPHICS::AnimationSystem::Init()
{
}

void NS_GRAPHICS::AnimationSystem::Exit()
{
}

unsigned NS_GRAPHICS::AnimationSystem::AddAnimController()
{
	size_t index = GetFreeIndex();

	//Full time to resize
	if (index >= _animControllers.size())
	{
		_animControllers.push_back(new AnimationController());
		_usedStatus.push_back(true);
	}
	else
	{
		_animControllers[index] = new AnimationController();
		_usedStatus[index] = true;
	}

	return (unsigned)index;
}

bool NS_GRAPHICS::AnimationSystem::RemoveAnimControllerByID(const int& index)
{
	if (_animControllers[index] == nullptr || index >= _animControllers.size())
	{
		return false;
	}

	_animControllers[index] = nullptr;
	_usedStatus[index] = false;

	return true;
}

size_t NS_GRAPHICS::AnimationSystem::GetFreeIndex()
{
	return std::distance(_usedStatus.begin(), std::find(_usedStatus.begin(), _usedStatus.end(), false));
}
