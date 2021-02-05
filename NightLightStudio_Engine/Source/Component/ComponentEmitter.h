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

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentEmitter();

	//read and write function for initialization from saved files
	void	Read(Value&);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentEmitter* Clone();

} EmitterComponent;
