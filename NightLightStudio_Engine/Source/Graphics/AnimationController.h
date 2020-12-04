#pragma once
#include <string>
#include <set>
#include "../Component/ComponentAnimation.h"

struct Model;

struct AnimationController
{
	std::string _currAnim;
	std::string _defaultAnim;

	std::set<std::string> _allAnims;

	bool _play;
	bool _loop;
	bool _isPlaying;

	double _dt;
	double _endAnimTime;

	void Update(float dt);
	void PlayAnimation(std::string newAnim, ComponentAnimation* currComp, bool loop = true);
};