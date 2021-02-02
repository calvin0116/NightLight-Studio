#pragma once
#include "..\glm\common.hpp"
#include "..\glm\gtc\constants.hpp"
#include <vector>
#include "Particle.h"

enum EmitterShapeType
{
	SPHERE, //ONLY THIS FOR NOW
	CONE,
	TOTAL_SHAPE
};

constexpr auto PI = 3.14159265358979323846;

class Emitter
{
public:
	std::vector<Particle> _particles;
	int _numAlive;
	unsigned _lastUsed;
	float _emitterTime;
	float _timePassed;

	glm::vec3 _emitterPosition;
	glm::vec3 _emitterRotation;
	glm::vec3 _emitterScale;

	EmitterShapeType _type;
	float _durationPerCycle;
	float _emissionRate;
	unsigned _maxParticles;

	float _spawnAngle;
	float _radius;

	bool _randomizeSize;
	float _minParticleSize;
	float _maxParticleSize;

	bool _randomizeSpeed;
	float _minParticleSpeed;
	float _maxParticleSpeed;

	bool _randomizeLifespan;
	float _minLifespan;
	float _maxLifespan;

	bool _play;
	bool _preWarm;
	bool _burst;
	bool _loop;
	bool _reverse;
	bool _follow;
	bool _fade;

	bool _velocityOverTime;
	bool _sizeOverTime;
	bool _speedOverTime;
	bool _colourOverTime;

	bool _randomizeColour;
	glm::vec4 _colourA;
	glm::vec4 _colourB;

	glm::vec4 _colourStart;
	glm::vec4 _colourEnd;

	glm::vec3 _velocity;

	unsigned _vao;
	unsigned _vbo;
	unsigned _ebo;
	unsigned _posBuffer;
	unsigned _colBuffer;

	std::vector<glm::vec4> _particlesPosition;
	std::vector<glm::vec4> _particlesColour;

	Emitter();
	~Emitter();

	void AddTime(float dt);
	float RandFloat();

	void InitParticles();
	void ResetParticle(size_t index);
	void RespawnParticle();
	unsigned FindUnused();
	

	// Comments
	glm::vec3 RandomVec3();
	

	// Comments please
	void InitBuffer();
	void UpdateBuffer();
	void SortParticle();
	void UpdateSize();

	void Play();
	void Stop();
	
};