#pragma once
#include "../Graphics/Lights.h"

typedef class ComponentLight //: public IComponent
{
public:
	// Temporarily make them public for easy access

	bool _isActive; // Temporarily set to true at beginning

	// value is -1 if no light is assigned
	int _lightID;

	// Type of light: POINT/DIRECTIONAL/SPOT
	NS_GRAPHICS::Lights _type;

	// Default constructor
	ComponentLight();

	// Parametrized constructor 
	ComponentLight(const int& lightID, const NS_GRAPHICS::Lights& Type);

	void AssignLight(const int& lightID, const NS_GRAPHICS::Lights& Type);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentLight();

	/////////// GETTERS/SETTERS ////////////

	// Applicable for all lights
	glm::vec3 GetAmbient() const;
	// Applicable for all lights
	void SetAmbient(const glm::vec3& ambient);

	// Applicable for all lights
	glm::vec3 GetDiffuse() const;
	// Applicable for all lights
	void SetDiffuse(const glm::vec3& diffuse);

	// Applicable for all lights
	glm::vec3 GetSpecular() const;
	// Applicable for all lights
	void SetSpecular(const glm::vec3& specular);

	////////////////////////////////////////

	//read and write function for initialization from saved files
	void Read();
	void Write();
} LightComponent;