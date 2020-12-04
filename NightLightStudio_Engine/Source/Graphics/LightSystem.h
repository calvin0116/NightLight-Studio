#pragma once

#include "../../framework.h"
#include "../Component/ComponentManager.h"
#include "../Component/ComponentLight.h"
#include <array>

#include "Lights.h"

namespace NS_GRAPHICS
{
	static const size_t s_MaxLights = 512;

	// 57424
	struct LightBlock
	{
		DirLight _dLight[1];
		PointLight _pLights[s_MaxLights];
		SpotLight _sLights[s_MaxLights];

		// Number of lights currently in scene
		int _dLights_Num;
		int _pLights_Num;
		int _sLights_Num;

		float _dummyPadding1 = 0.f;

		glm::vec4 _viewPos;

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

		float _gamma = 2.2f;

	public:
		// Unique Singleton instance
		static LightSystem& GetInstance()
		{
			static LightSystem instance;
			return instance;
		}

		void Init();

		void Update();

		// Get lightblock from lightsystem
		LightBlock*& GetLightBlock();

		void SortLights(const Lights& lightType, const int& deletedIndex);

		void SetAllDirectionalLights(const bool& setter);

		void SetAllPointLights(const bool& setter);

		void SetAllLights(const bool& setter);

		// Return true if light is proper set, else return false
		bool SetLight(const Lights& type, ComponentLight* const& comp);

		// Sets directional light to provided component
		void SetDirectionalLight(ComponentLight* const& comp);

		// Sets point light to provided component
		void SetPointLight(ComponentLight* const& comp);

		// Sets spot light to provided component
		void SetSpotLight(ComponentLight* const& comp);

		// Removes tracker from light component / deletes light
		void RemoveLight(ComponentLight* const& comp);

		// Sets active for component, requires special functionality due to uniform block
		// uses inactive type to store active state properties
		void SetActive(ComponentLight* const& comp, const bool& set);

		// Remove light helper
		void RemoveLightHelper(const Lights& type, const int& id);

		// Remove all lights from scene (Used upon scene exit)
		void RemoveAllLights();

		void SetGamma(const float& gamma = 2.2f);

		float GetGamma() const;
	};
}