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
	//////////////////////////////////

	// Default constructor
	ComponentLight();

	// Destructor
	~ComponentLight();

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

	bool SetType(const NS_GRAPHICS::Lights& type); // Expose

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
	void Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override;
	Value	Write();
	virtual Value& Write(Value& val) { return val; };
	virtual ComponentLight* Clone()
	{
		ComponentLight* newcomp = new ComponentLight();
		*newcomp = *this;
		return newcomp;
	}
} LightComponent;