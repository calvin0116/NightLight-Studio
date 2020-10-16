#pragma once
#include "Components.h"
#include "..\Graphics\Mesh.h"

class ComponentGraphics : public IComponent
{
public:
	// Temporarily make them public for easy access

	// Empty mesh at start
	NS_GRAPHICS::Mesh mesh;

	// Default constructor
	ComponentGraphics();

	// Parametrized constructor 
	ComponentGraphics(const NS_GRAPHICS::Mesh& reference);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentGraphics();

	//read and write function for initialization from saved files
	void Read();
	void Write();
};