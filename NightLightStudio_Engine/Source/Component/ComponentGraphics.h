#pragma once
#include <string>

#include "..\\..\\ISerializable.h"

typedef class ComponentGraphics : public ISerializable//: public IComponent
{
public:
	// Temporarily make them public for easy access

	bool isActive; // Temporarily set to true at beginning

	//Serializing doesnt really save IDS? i think might have to add in a file name as well
	unsigned MeshID;

	std::string _textureFileName;
	unsigned _textureID; // Temporarily only diffuse texture

	// Default constructor
	ComponentGraphics();

	// Parametrized constructor 
	ComponentGraphics(const unsigned& meshID);

	void AssignMeshID(const unsigned& meshID);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentGraphics();

	//read and write function for initialization from saved files
	virtual void	Read(Value& ) {  };
	virtual Value	Write() { return Value(); };
	virtual Value& Write(Value& val) { return val; };
} GraphicsComponent;