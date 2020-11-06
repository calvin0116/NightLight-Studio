#pragma once
#include <string>
#include "..\glm\glm.hpp"
#include "..\\..\\ISerializable.h"
#include "LocalString.h"

// Designer Manual Input
struct MaterialData
{
	glm::vec3 _diffuse;  // Base color
	glm::vec3 _ambient;  // Reflective color under ambient lighting
	glm::vec3 _specular; // Highlight color
	float _shininess;    // Scattering / radius of specularity

	MaterialData()
		: _diffuse{ 0.5f,0.5f,0.5f },
		_ambient{ 1.f, 1.f, 1.f },
		_specular{ 0.5f,0.5f,0.5f },
		_shininess{ 32.f } {}

	~MaterialData() {}
};

typedef class ComponentGraphics : public ISerializable//: public IComponent
{
public:
	// Temporarily make them public for easy access

	bool _isActive; // Temporarily set to true at beginning

	//Serializing doesnt really save IDS? i think might have to add in a file name as well
	//int MeshID; // value is -1 if no mesh is assigned
	//std::string _meshFileName;
	//LocalString<256> _meshFileName;

	int _modelID; // value is -1 if no mesh is assigned
	LocalString<256> _modelFileName;

	/////////////////////////////////////////
	/// Mapping Variables
	/////////////////////////////////////////
	//This is colour/albedo/diffuse
	LocalString<256> _albedoFileName;
	unsigned _albedoID;

	LocalString<256> _normalFileName;
	unsigned _normalID;

	LocalString<256> _metallicFileName;
	unsigned _metallicID;

	LocalString<256> _roughnessFileName;
	unsigned _roughnessID;

	//Ambient Occlusion
	LocalString<256> _aoFileName;
	unsigned _aoID;

	/////////////////////////////////////////
	/// Material without texture
	/////////////////////////////////////////
	MaterialData _materialData;

	// Default constructor
	ComponentGraphics();

	// Parametrized constructor 
	ComponentGraphics(const int& meshID);

	void AttachMesh(const int& meshID);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentGraphics();

	//read and write function for initialization from saved files
	void	Read(Value&);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentGraphics* Clone()
	{
		ComponentGraphics* newcomp = new ComponentGraphics();
		*newcomp = *this;
		return newcomp;
	}
} GraphicsComponent;