#pragma once


typedef class ComponentGraphics //: public IComponent
{
public:
	// Temporarily make them public for easy access

	bool isActive; // Temporarily set to true at beginning

	unsigned MeshID;

	// Default constructor
	ComponentGraphics();

	// Parametrized constructor 
	ComponentGraphics(const unsigned& meshID);

	void AssignMeshID(const unsigned& meshID);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentGraphics();

	//read and write function for initialization from saved files
	void Read();
	void Write();
} GraphicsComponent;