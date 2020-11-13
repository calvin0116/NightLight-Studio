#pragma once
#include "../Graphics/Lights.h"
#include "..\\..\\ISerializable.h"

typedef class ComponentLight : public ISerializable //: public IComponent
{
	// Inactive/Active data
	// Used to store initial light type for activation and deactivation
	NS_GRAPHICS::Lights _inactive_type = NS_GRAPHICS::Lights::INVALID_TYPE;

public:
	// Temporarily make them public for easy access

	bool _isActive; // Temporarily set to true at beginning // Expose

	// value is -1 if no light is assigned
	int _lightID;

	// Type of light: POINT/DIRECTIONAL/SPOT
	NS_GRAPHICS::Lights _type;

	//Variables For Lighting
	//Standard variable
	glm::vec3 _ambient;   // Expose
	glm::vec3 _diffuse;   // Expose
	glm::vec3 _specular;  // Expose

	// For spot and directional
	// used as reference only
	// Note that direction is set based on transform component's rotation
	glm::vec3 _direction;

	// For point and spot
	// Also known as intensity
	float _intensity; // Expose

	// For spot
	float _cutOff;        // Expose
	float _outerCutOff;   // Expose

	// Default constructor
	ComponentLight();

	// Parametrized constructor 
	ComponentLight(const int& lightID, const NS_GRAPHICS::Lights& Type);

	// Assume create light function is called separately by light system
	void AssignLight(const int& lightID, const NS_GRAPHICS::Lights& Type);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentLight();

	/////////// GETTERS/SETTERS ////////////

	// Must use this function for setting active/inactive for light
	void SetActive(const bool& set);

	bool GetActive() const;

	NS_GRAPHICS::Lights GetInactiveType() const;

	void ChangeLightType(const NS_GRAPHICS::Lights& Type);  // Expose

	// Applicable for all lights
	glm::vec3 GetAmbient() const;               // Expose
	// Applicable for all lights
	void SetAmbient(const glm::vec3& ambient);  // Expose

	// Applicable for all lights
	glm::vec3 GetDiffuse() const;               // Expose
	// Applicable for all lights
	void SetDiffuse(const glm::vec3& diffuse);  // Expose

	// Applicable for all lights
	glm::vec3 GetSpecular() const;              // Expose
	// Applicable for all lights
	void SetSpecular(const glm::vec3& specular); // Expose

	// Applicable for spot and spot light
	// Calculates intensity based on current attenuation
	float GetIntensity();                         // Expose
	// Calculates attenuation based on provided intensity
	void SetIntensity(const float& intensity);     // Expose

	// Alternate way to set attenuation/intensity
	float GetAttenuation();                         // Expose
	void SetAttenuation(const float& attenuation);  // Expose


	//TO DO
	// Set setters and getters for spotlight

	////////////////////////////////////////

	//read and write function for initialization from saved files
	void Read(Value& val);
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentLight* Clone()
	{
		ComponentLight* newcomp = new ComponentLight();
		*newcomp = *this;
		return newcomp;
	}
} LightComponent;