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
		: _diffuse{ 1.0f,1.0f,1.0f },
		_ambient{ 1.0f, 1.0f, 1.0f },
		_specular{ 1.0f,1.0f,1.0f },
		_shininess{ 32.f } {}

	~MaterialData() {}
};

struct PBRMaterialData
{
	glm::vec3 _albedo;  // Base color
	float _metallic; // from 0.f to 1.f
	float _roughness;

	PBRMaterialData()
		: _albedo{ 1.0f, 1.0f, 1.0f },
		_metallic{ 1.0f },
		_roughness{ 0.5f }{}
};

enum class RENDERTYPE
{
	SOLID = 0,
	TEXTURED
};

typedef class ComponentGraphics : public ISerializable//: public IComponent
{
	float _alpha = 1.f;

public:
	// Temporarily make them public for easy access
	RENDERTYPE _renderType = RENDERTYPE::SOLID;

	bool _isActive; // Temporarily set to true at beginning // Expose

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

	LocalString<256> _specularFileName;
	unsigned _specularID;

	/////////////////////////////////////////
	/// Material without texture
	/////////////////////////////////////////
	MaterialData _materialData;

	PBRMaterialData _pbrData;

	// Default constructor
	ComponentGraphics();

	// Parametrized constructor 
	ComponentGraphics(const int& meshID);

	void AttachMesh(const int& meshID); // Help expose Alex
	int GetCurrentMeshHandle(); // Help expose Alex

	void SetRenderType(const RENDERTYPE& rendertype);

	void AddModel(std::string filename);              // Expose
	void AddAlbedoTexture(std::string filename);      // Expose
	void AddNormalTexture(std::string filename);      // Expose
	void AddMetallicTexture(std::string filename);    // Expose
	void AddRoughnessTexture(std::string filename);   // Expose
	void AddAOTexture(std::string filename);          // Expose
	void AddSpecularTexture(std::string filename);    // Expose

	void SetAlpha(const float& alpha);
	float GetAlpha() const;

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