#pragma once
#include "../Graphics/Lights.h"
#include "..\\..\\ISerializable.h"

typedef class ComponentLight : public ISerializable //: public IComponent
{
	//////////////////////////////////
	///// Updated variables for PBR
	//////////////////////////////////

	// Temporarily set to true at beginning
	// MUST NOT BE ACCESSED DIRECTLY
	bool _isActive;

	// value is -1 if no light is assigned
	// used to point to designated light in uniform block
	int _lightID;

	// Type of light: POINT/DIRECTIONAL/SPOT
	NS_GRAPHICS::Lights _type;

	// Color of light
	glm::vec3 _PBRdiffuse;

	// For point and spot
	float _intensity; // Expose

	// For spot and directional
	// used as reference only
	// Note that direction is set based on transform component's rotation
	glm::vec3 _direction;

	// for point light
	float _radius;

	// For spot
	float _cutOff;        // Expose
	float _outerCutOff;   // Expose

	// Inactive/Active data
	// Used to store initial light type for activation and deactivation
	NS_GRAPHICS::Lights _inactiveType = NS_GRAPHICS::Lights::INVALID_TYPE;

	//////////////////////////////////

public:
	// Temporarily make them public for easy access

	//Variables For Lighting
	//Standard variable
	//glm::vec3 _ambient;  
	//glm::vec3 _diffuse;  
	//glm::vec3 _specular; 

	

	//////////////////////////////////

	// Default constructor
	ComponentLight();

	// Parametrized constructor 
	//ComponentLight(const int& lightID, const NS_GRAPHICS::Lights& Type);

	// Assume create light function is called separately by light system
	//void AssignLight(const int& lightID, const NS_GRAPHICS::Lights& Type);

	// Destructor, destroy all OpenGL objects via graphics system
	~ComponentLight();

	/////////// GETTERS/SETTERS ////////////

	// Must use this function for setting active/inactive for light
	//void SetActive(const bool& set);            // Expose

	//

	//void ChangeLightType(const NS_GRAPHICS::Lights& Type);  // Expose

	//// Applicable for all lights
	//glm::vec3 GetAmbient() const;               // Expose
	//// Applicable for all lights
	//void SetAmbient(const glm::vec3& ambient);  // Expose

	//// Applicable for all lights
	//glm::vec3 GetDiffuse() const;               // Expose
	//// Applicable for all lights
	//void SetDiffuse(const glm::vec3& diffuse);  // Expose

	//// Applicable for all lights
	//glm::vec3 GetSpecular() const;              // Expose
	//// Applicable for all lights
	//void SetSpecular(const glm::vec3& specular); // Expose

	//// Applicable for spot and spot light
	//// Calculates intensity based on current attenuation
	////float GetIntensity();                         // Expose
	//// Calculates attenuation based on provided intensity
	//void SetIntensity(const float& intensity);     // Expose

	//TO DO
	// Set setters and getters for spotlight

	///////////////////////////////
	/////NEW INTERFACE FOR LIGHTS
	///////////////////////////////

	// Getters
	bool GetActive() const; // Expose for all lights

	NS_GRAPHICS::Lights GetType() const; // Expose

	NS_GRAPHICS::Lights GetInactiveType() const;

	int GetLightID() const;

	glm::vec3 GetColor(); // Expose for all lights

	float GetIntensity(); // Expose for all lights

	// Direction vector here is transformed vector after model transform
	// Does not require setter, a constant vector (1.f,0.f,0.f) is transformed
	//glm::vec3 GetDirectionVector() const; // Expose, but do not allow alteration

	// Radius of point light
	float GetRadius() const; // Expose

	// Angle which represents the radius of the outer spotlight radius of light
	// Used for gradual falloff
	float GetOuterCutOff() const; // Expose

	// This is the angle which specifies the spotlight radius of light
	// https://learnopengl.com/Lighting/Light-casters
	float GetCutOff() const; // Expose

	// Setters
	void SetLightID(const int& id); // DO NOT EXPOSE

	void SetType(const NS_GRAPHICS::Lights& type); // Expose

	// Note: FOR LIGHT SYSTEM USAGE ONLY
	void LS_SetType(const NS_GRAPHICS::Lights& type); // DO NOT EXPOSE

	void SetColor(const glm::vec3& color); // Expose

	void SetIntensity(const float& intensity); // expose

	void SetActive(const bool& set); // expose

	// Note: FOR LIGHT SYSTEM USAGE ONLY
	void LS_SetActive(const bool& set); // DO NOT EXPOSE

	void SetRadius(const float& radius); // Expose

	void SetOuterCutOff(const float& outercutoff); // Expose

	void SetCutOff(const float& cutoff); // Expose

	///////////////////////////////


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