#pragma once
#include "..\glm\glm.hpp"

struct Particle
{
	glm::vec3 _position;
	glm::vec3 _velocity;
	glm::vec4 _colourStart;
	glm::vec4 _colourEnd;
	float _timeAlive;
	float _lifespan;
	float _size;
	float _speed;
	float _cameraDistance;
	bool _alive;

	Particle();
	~Particle() = default;

	bool operator<(const Particle& rhs) const;
};
