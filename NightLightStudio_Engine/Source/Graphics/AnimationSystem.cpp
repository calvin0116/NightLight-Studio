#include "AnimationSystem.h"
#include "GraphicsSystem.h"

NS_GRAPHICS::AnimationSystem::AnimationSystem()
{
}

NS_GRAPHICS::AnimationSystem::~AnimationSystem()
{
}

void NS_GRAPHICS::AnimationSystem::Update()
{
	for (auto& anim : _animControllers)
	{
		anim.second->Update(DELTA_T->real_dt * DT_SCALE);
	}
}

void NS_GRAPHICS::AnimationSystem::Free()
{
	for (auto& anim : _animControllers)
	{
		delete anim.second;
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
	_animControllers.insert(std::make_pair( _animIndex , new AnimationController()));
	return _animIndex++;
}
