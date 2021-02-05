#include "Particle.h"

Particle::Particle() : _position{ 0.0f,0.0f,0.0f }, _velocity{ 0.0f,0.0f,0.0f }, _colourStart{ 1.0f,1.0f,1.0f,1.f },
_colourEnd{ 1.0f,1.0f,1.0f,1.f }, _timeAlive{ 0.0f }, _lifespan{ 0.0f }, _size{ 1.0f }, _speed{ 0.0f }, _cameraDistance{ -1.0f },
_alive{ false }
{

}

bool Particle::operator<(const Particle& rhs) const
{
	return _cameraDistance > rhs._cameraDistance;
}
