#pragma once
#include <string>

typedef class ComponentGraphics //: public IComponent
{
public:
	// Temporarily make them public for easy access

	bool _isActive; // Temporarily set to true at beginning

	//Serializing doesnt really save IDS? i think might have to add in a file name as well
	int MeshID; // value is -1 if no mesh is assigned
	std::string _meshFileName;

	std::string _textureFileName;
	unsigned _textureID; // Temporarily only diffuse texture

	// Default constructor
	ComponentGraphics();

	// Parametrized constructor 
	ComponentGraphics(const int& meshID);

	void AttachMesh(const int& meshID);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentGraphics();

	//read and write function for initialization from saved files
	void Read();
	void Write();
} GraphicsComponent;