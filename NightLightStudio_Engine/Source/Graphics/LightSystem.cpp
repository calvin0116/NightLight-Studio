#include "LightSystem.h"
#include "../Component/ComponentLight.h"
#include "../Component/ComponentTransform.h"
#include "ShaderSystem.h"

namespace NS_GRAPHICS
{
	LightSystem::LightSystem()
	{
		lightblock = new LightBlock();
		lightblock->_dLights_Num = 0;
		lightblock->_sLights_Num = 0;
		lightblock->_pLights_Num = 0;
	}

	LightSystem::~LightSystem()
	{
		delete lightblock;
	}

	void LightSystem::Init()
	{
		// Set a default gamma value
		SetGamma(1.8f);
	}

	void LightSystem::Update()
	{
		// Push transforms into existing lights
		/* The following must be updated each time this is run:
		   - Direction vector based on model transformation from transform component(if any)
		   - Position based on model transformation component(except directional light)
		*/
		auto itr = G_ECMANAGER->begin<ComponentLight>();
		auto itrEnd = G_ECMANAGER->end<ComponentLight>();
		while (itr != itrEnd)
		{
			ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

			if (lightcomp->GetLightID() != -1)
			{
				// get transform component
				ComponentTransform* transformComp = G_ECMANAGER->getEntity(itr).getComponent<ComponentTransform>();

				if (transformComp == nullptr)
					continue;

				// Get specified light from light block
				// Dammit I would make a base class to make things more readable but it'll mess up with the light block alignment
				if (lightcomp->GetType() == Lights::DIRECTIONAL)
				{
					DirLight& dirlight = lightblock->_dLight[lightcomp->GetLightID()];
					dirlight._direction = transformComp->GetModelMatrix() * glm::vec4(1.f, 0.f, 0.f, 0.f); // w = 0 for vector
					dirlight._diffuse = glm::vec4(lightcomp->GetColor(), 1.f);
					dirlight._intensity = lightcomp->GetIntensity();
				}
				else if (lightcomp->GetType() == Lights::POINT)
				{
					PointLight& pointlight = lightblock->_pLights[lightcomp->GetLightID()];
					pointlight.position = transformComp->GetModelMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f); // w = 1 for point
					pointlight._intensity = lightcomp->GetIntensity();
					pointlight._diffuse = glm::vec4(lightcomp->GetColor(), 1.f);
					pointlight._radius = lightcomp->GetRadius();
				}
				else if (lightcomp->GetType() == Lights::SPOT)
				{
					SpotLight& spotlight = lightblock->_sLights[lightcomp->GetLightID()];
					spotlight._direction = transformComp->GetModelMatrix() * glm::vec4(1.f, 0.f, 0.f, 0.f); // w = 0 for vector
					spotlight.position = transformComp->GetModelMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f); // w = 1 for point
					spotlight._intensity = lightcomp->GetIntensity();
					spotlight._diffuse = glm::vec4(lightcomp->GetColor(), 1.f);
					spotlight._cutOff = lightcomp->GetCutOff();
					spotlight._outerCutOff = lightcomp->GetOuterCutOff();
				}
			}
			++itr;
		}
	}

	LightBlock*& LightSystem::GetLightBlock()
	{
		return lightblock;
	}

	void LightSystem::SortLights(const Lights& lightType, const int& deletedIndex)
	{
		auto itr = G_ECMANAGER->begin<ComponentLight>();
		auto itrEnd = G_ECMANAGER->end<ComponentLight>();

		if (lightType == Lights::POINT)
		{
			// Decrement all valid light component IDs more than deleted index by 1 to pack tightly
			while (itr != itrEnd)
			{
				ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

				if (lightcomp->GetLightID() != -1 && lightcomp->GetType() == Lights::POINT && lightcomp->GetLightID() > deletedIndex)
				{
					lightcomp->SetLightID(lightcomp->GetLightID() - 1);
				}
				++itr;
			}

			// Shift all  directional lights in array by 1 to the left
			for (int i = deletedIndex + 1; i < lightblock->_pLights_Num; ++i)
				lightblock->_pLights[i - 1] = lightblock->_pLights[i];

			lightblock->_pLights_Num--; // decrement total number of lights
		}
		else if(lightType == Lights::SPOT)
		{
			// Decrement all valid light component IDs more than deleted index by 1 to pack tightly
			while (itr != itrEnd)
			{
				ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

				if (lightcomp->GetLightID() != -1 && lightcomp->GetType() == Lights::SPOT && lightcomp->GetLightID() > deletedIndex)
				{
					lightcomp->SetLightID(lightcomp->GetLightID() - 1);
				}
				++itr;
			}

			// Shift all  directional lights in array by 1 to the left
			for (int i = deletedIndex + 1; i < lightblock->_sLights_Num; ++i)
				lightblock->_sLights[i - 1] = lightblock->_sLights[i];

			lightblock->_sLights_Num--; // decrement total number of lights
		}
	}

	void LightSystem::SetAllDirectionalLights(const bool& setter)
	{
		auto itr = G_ECMANAGER->begin<ComponentLight>();
		auto itrEnd = G_ECMANAGER->end<ComponentLight>();

		while (itr != itrEnd)
		{
			ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

			if (lightcomp->GetInactiveType() == Lights::DIRECTIONAL)
				lightcomp->SetActive(setter);

				++itr;
		}
	}

	void LightSystem::SetAllPointLights(const bool& setter)
	{
		auto itr = G_ECMANAGER->begin<ComponentLight>();
		auto itrEnd = G_ECMANAGER->end<ComponentLight>();

		while (itr != itrEnd)
		{
			ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

			if (lightcomp->GetInactiveType() == Lights::POINT)
				lightcomp->SetActive(setter);

			++itr;
		}
	}

	void LightSystem::SetAllLights(const bool& setter)
	{
		auto itr = G_ECMANAGER->begin<ComponentLight>();
		auto itrEnd = G_ECMANAGER->end<ComponentLight>();

		while (itr != itrEnd)
		{
			ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

			lightcomp->SetActive(setter);

			++itr;
		}
	}

	bool LightSystem::SetLight(const Lights& type, ComponentLight* const& comp)
	{
		switch (type)
		{
		case Lights::DIRECTIONAL:
			if (lightblock->_dLights_Num)
				return false;

			SetDirectionalLight(comp);
			return true;
			break;

		case Lights::POINT:
			if (lightblock->_pLights_Num >= s_MaxLights)
				return false;

			SetPointLight(comp);
			return true;
			break;

		case Lights::SPOT:
			if (lightblock->_sLights_Num >= s_MaxLights)
				return false;

			SetSpotLight(comp);
			return true;
			break;

		case Lights::INVALID_TYPE:
			RemoveLight(comp);
			return true;
			break;
		}

		return false;
	}

	void LightSystem::SetDirectionalLight(ComponentLight* const& comp)
	{
		if (comp->GetType() == Lights::DIRECTIONAL)
			return;

		// If light exists 
		if (comp->GetLightID() != -1)
			RemoveLight(comp); // Remove light from uniform component

		// Add new directional light into list
		comp->SetLightID(lightblock->_dLights_Num++);

		comp->LS_SetType(Lights::DIRECTIONAL);

		// Does not require data copy from component, this is done in lights update
	}

	void LightSystem::SetPointLight(ComponentLight* const& comp)
	{
		if (comp->GetType() == Lights::POINT)
			return;

		// If light exists 
		if (comp->GetLightID() != -1)
			RemoveLight(comp); // Remove light from uniform component

		// Add new point light into list
		comp->SetLightID(lightblock->_pLights_Num++);

		comp->LS_SetType(Lights::POINT);

		// Does not require data copy from component, this is done in lights update
	}

	void LightSystem::SetSpotLight(ComponentLight* const& comp)
	{
		if (comp->GetType() == Lights::SPOT)
			return;

		// If light exists 
		if (comp->GetLightID() != -1)
			RemoveLight(comp); // Remove light from uniform component

		// Add new point light into list
		comp->SetLightID(lightblock->_sLights_Num++);

		comp->LS_SetType(Lights::SPOT);

		// Does not require data copy from component, this is done in lights update
	}

	void LightSystem::RemoveLight(ComponentLight* const& comp)
	{
		if (comp->GetLightID() == -1 && comp->GetType() == Lights::INVALID_TYPE)
			return;

		RemoveLightHelper(comp->GetType(), comp->GetLightID());

		// Remove id from component
		comp->SetLightID(-1);

		// Set light type to invalid type
		comp->LS_SetType(Lights::INVALID_TYPE);
	}

	void LightSystem::SetActive(ComponentLight* const& comp, const bool& set)
	{
		/* 
		   Due to the nature of the uniform block,
		   we must "remove" the light to deactivate it,
		   then "add" the light again to activate it.
		   Thus, we must store the existing list type privately first.
		   Upon reactivation, add the light back again with existing data in component
		*/
		// Check that component has valid light assigned and set is different from current
		if (set != comp->GetActive() && comp->GetInactiveType() != Lights::INVALID_TYPE)
		{
			if (set == true)
				SetLight(comp->GetInactiveType(), comp);
			else
				SetLight(Lights::INVALID_TYPE, comp);

			comp->LS_SetActive(set);
		}
	}

	void LightSystem::RemoveLightHelper(const Lights& type, const int& id)
	{
		if (id != -1)
		{
			if (type == Lights::DIRECTIONAL)
			{
				// Check if light is the last on the list
				// if yes, just delete by reducing light count
				if (id == lightblock->_dLights_Num - 1)
				{
					lightblock->_dLights_Num--;
				}
				else
				{
					// else, sort lights accordingly
					SortLights(type, id);
				}
			}
			else if (type == Lights::POINT)
			{
				// Check if light is the last on the list
				// if yes, just delete by reducing light count
				if (id == lightblock->_pLights_Num - 1)
				{
					lightblock->_pLights_Num--;
				}
				else
				{
					// else, sort lights accordingly
					SortLights(type, id);
				}
			}
			else if (type == Lights::SPOT)
			{
				// Check if light is the last on the list
				// if yes, just delete by reducing light count
				if (id == lightblock->_sLights_Num - 1)
				{
					lightblock->_sLights_Num--;
				}
				else
				{
					// else, sort lights accordingly
					SortLights(type, id);
				}
			}
		}
	}

	void LightSystem::RemoveAllLights()
	{
		lightblock->_dLights_Num = 0;
		lightblock->_pLights_Num = 0;
		lightblock->_sLights_Num = 0;
	}

	void LightSystem::SetGamma(const float& gamma)
	{
		_gamma = gamma;

		// Four shader programs to account for
		NS_GRAPHICS::ShaderSystem* shaderSystem = &NS_GRAPHICS::ShaderSystem::GetInstance();

		shaderSystem->StartProgram(ShaderSystem::PBR_LIGHTPASS);
		glUniform1f(shaderSystem->GetGammaLocation(), _gamma);

		/*shaderSystem->StartProgram(ShaderSystem::PBR_ANIMATED);
		glUniform1f(shaderSystem->GetGammaLocation(), _gamma);

		shaderSystem->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
		glUniform1f(shaderSystem->GetGammaLocation(), _gamma);

		shaderSystem->StartProgram(ShaderSystem::PBR);
		glUniform1f(shaderSystem->GetGammaLocation(), _gamma);

		shaderSystem->StartProgram(ShaderSystem::PBR_TEXTURED);
		glUniform1f(shaderSystem->GetGammaLocation(), _gamma);
		
		shaderSystem->StartProgram(ShaderSystem::PBR_TEXTURED_NONORMALMAP);
		glUniform1f(shaderSystem->GetGammaLocation(), _gamma);

		shaderSystem->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED_NONORMALMAP);
		glUniform1f(shaderSystem->GetGammaLocation(), _gamma);*/

		shaderSystem->StopProgram();
	}
	float LightSystem::GetGamma() const
	{
		return _gamma;
	}
}