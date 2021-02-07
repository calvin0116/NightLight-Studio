#pragma once
#include <string>
#include "..\glm\glm.hpp"
#include "..\\..\\ISerializable.h"
#include "LocalString.h"

typedef class ComponentEmitter : public ISerializable//: public IComponent
{
public:
	bool _isActive; // Temporarily set to true at beginning // Expose

	int _emitterID; // value is -1 if no emitter is assigned

	LocalString<256> _image;
	unsigned _imageID;

	// Default constructor
	ComponentEmitter();

	// Parametrized constructor 
	ComponentEmitter(const int& emitterID);

	//Expose this also
	void SetEmitterID(const int& emitterID); // Help expose Alex
	int GetEmitterID(); // Help expose Alex

	void AddTexture(std::string filename);// Expose
	void Play();// Help expose Alex
	void Stop();// Help expose Alex

	EmitterShapeType GetShapeType();
	void SetShapeType(EmitterShapeType shape);

	float GetDuration();
	void SetDuration(float duration);

	unsigned GetEmissionRate();
	void SetEmissionRate(unsigned emission);

	unsigned GetMaxParticle();
	void SetMaxParticle(unsigned max);

	float GetBurstRate();
	void SetBurstRate(float burst);

	unsigned GetBurstAmount();
	void SetBurstAmount(unsigned amount);

	float GetAngle();
	void SetAngle(float angle);

	float GetRadius();
	void SetRadius(float radius);

	bool GetRandomizeSize();
	void SetRandomizeSize(bool size);

	float GetMinParticleSize();
	void SetMinParticleSize(float min);

	float GetMaxParticleSize();
	void SetMaxParticleSize(float max);

	bool GetRandomizeSpeed();
	void SetRandomizeSpeed(bool speed);

	float GetMinParticleSpeed();
	void SetMinParticleSpeed(float min);

	float GetMaxParticleSpeed();
	void SetMaxParticleSpeed(float max);

	bool GetRandomizeLife();
	void SetRandomizeLife(bool life);

	float GetMinParticleLife();
	void SetMinParticleLife(float min);

	float GetMaxParticleLife();
	void SetMaxParticleLife(float max);

	bool GetPrewarm();
	void SetPrewarm(bool prewarm);

	bool GetBurst();
	void SetBurst(bool burst);

	bool GetLoop();
	void SetLoop(bool loop);

	bool GetFollow();
	void SetFollow(bool follow);

	bool GetFade();
	void SetFade(bool fade);

	bool GetVelocityOverTime();
	void SetVelocityOverTime(bool vel);

	bool GetSizeOverTime();
	void SetSizeOverTime(bool size);

	bool GetSpeedOverTime();
	void SetSpeedOverTime(bool speed);

	bool GetColourOverTime();
	void SetColourOverTime(bool colour);

	glm::vec4 GetColourA();
	void SetColourA(glm::vec4 colour);

	glm::vec4 GetColourB();
	void SetColourB(glm::vec4 colour);

	glm::vec4 GetColourStart();
	void SetColourStart(glm::vec4 colour);

	glm::vec4 GetColourEnd();
	void SetColourEnd(glm::vec4 colour);

	glm::vec3 GetVelocity();
	void SetVelocity(glm::vec3 velocity);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentEmitter();

	//read and write function for initialization from saved files
	void	Read(Value&);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentEmitter* Clone();

} EmitterComponent;
