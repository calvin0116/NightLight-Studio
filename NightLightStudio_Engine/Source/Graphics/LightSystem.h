#pragma once

#include "../../framework.h"
#include "../Component/ComponentManager.h"
#include "../Component/ComponentLight.h"
#include <array>

#include "Lights.h"

namespace NS_GRAPHICS
{
	static const size_t s_MaxLights = 30;

	struct LightBlock
	{
		// 7200 total
		DirLight _dLights[s_MaxLights];		// 1920
		PointLight _pLights[s_MaxLights];   // 2400
		SpotLight _sLights[s_MaxLights];	// 2880
		// 0 - 7199

		// Number of lights currently in scene
		int _dLights_Num; // 7200 - 7203
		int _pLights_Num; // 7204 - 7207
		int _sLights_Num; // 7208 - 7211
		// 7216
		
		// Compiler will align view pos to 7216
		glm::vec4 _viewPos; // 7216 - 7231
		// Extra 4 bytes of padding?
		float _dummyPadding0 = 0.f;

		LightBlock()
			: _dLights_Num{ NULL },
			_pLights_Num{ NULL },
			_sLights_Num{ NULL },
			_viewPos()
		{}
		~LightBlock() = default;
	};

	class LightSystem
	{
		LightSystem();
		~LightSystem();

		LightBlock* lightblock;

		// Light block trackers
		// Used to check for available/allocated lights for light components
		// All set to false upon initialization
		std::array<bool, s_MaxLights> dLights_tracker{};
		std::array<bool, s_MaxLights> pLights_tracker{};
		std::array<bool, s_MaxLights> sLights_tracker{};

	public:
		// Unique Singleton instance
		static LightSystem& GetInstance()
		{
			static LightSystem instance;
			return instance;
		}

		void Init();

		void Update();

		/* TO DO*/
		// Position should be set in transform component
		// Update uniforms for lights based on existing data
		// Get lightblock from lightsystem
		LightBlock*& GetLightBlock();

		// Adds Directional Light to scene, returns the id token for currently added light
		int AddDirLight(const glm::vec3& direction = glm::vec3(1.f, 0.f, 0.f), const glm::vec3& ambient = glm::vec3(0.2f, 0.2f, 0.2f),
			const glm::vec3& diffuse = glm::vec3(0.5f, 0.5f, 0.5f), const glm::vec3& specular = glm::vec3(1.f, 1.f, 1.f));

		// Adds Point Light to scene, returns the id token for currently added light
		int AddPointLight(const float& attenuation = 0.2f, const glm::vec3& ambient = glm::vec3(1.f, 1.f, 1.f),
			const glm::vec3& diffuse = glm::vec3(1.f, 1.f, 1.f), const glm::vec3& specular = glm::vec3(1.f, 1.f, 1.f));

		// Adds Spot Light to scene, returns the id token for currently added light
		int AddSpotLight(const glm::vec3& direction = glm::vec3(1.f, 0.f, 0.f), const float& cutoff = 1.f, const float& outercutoff = 100.f,
			const float& attenuation = 100.f, const glm::vec3& ambient = glm::vec3(1.f, 1.f, 1.f),
			const glm::vec3& diffuse = glm::vec3(1.f, 1.f, 1.f), const glm::vec3& specular = glm::vec3(1.f, 1.f, 1.f));

		// Creates and attaches light component to entity
		// Existing light attached to entity will be deleted
		void AttachLightComponent(Entity& entity, Lights lightType = Lights::DIRECTIONAL);

		// Detaches and deletes light component attached to entity if it exists
		void DetachLightComponent(Entity& entity);

		// Just in case component is destroyed before detach light component is called
		//void RemoveLight(const int& id, Lights lightType);

		void SortLights(const Lights& lightType, const int& deletedIndex);
		
		// TO DO: All getters and setters should be done in system
		void ChangeLightType(ComponentLight* light, Lights lightType);
		
		DirLight& GetDirLight(const int& id);
		SpotLight& GetSpotLight(const int& id);
		PointLight& GetPointLight(const int& id);
	};
}
