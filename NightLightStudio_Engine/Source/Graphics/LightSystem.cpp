#include "LightSystem.h"
#include "../Component/ComponentLight.h"
#include "../Component/ComponentTransform.h"

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
		// Test add 1 light of each type
		// TEST
		//AddDirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Somewhat working now?
		//AddDirLight(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Somewhat working now?
		//AddPointLight(0.2f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Now this works
		//AddSpotLight(glm::vec3(-1.0f, 0.0f, 0.f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)),
					//0.2f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
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
					DirLight& dirlight = lightblock->_dLights[lightcomp->GetLightID()];
					dirlight._direction = transformComp->GetModelMatrix() * glm::vec4(1.f, 0.f, 0.f, 0.f); // w = 0 for vector
					//dirlight._ambient = glm::vec4(lightcomp->_ambient, 1.f);
					dirlight._diffuse = glm::vec4(lightcomp->GetColor(), 1.f);
					//dirlight._specular = glm::vec4(lightcomp->_diffuse, 1.f);
				}
				else if (lightcomp->GetType() == Lights::POINT)
				{
					PointLight& pointlight = lightblock->_pLights[lightcomp->GetLightID()];
					pointlight.position = transformComp->GetModelMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f); // w = 1 for point
					pointlight._intensity = lightcomp->GetIntensity();
					//pointlight._ambient = glm::vec4(lightcomp->_ambient, 1.f);
					pointlight._diffuse = glm::vec4(lightcomp->GetColor(), 1.f);
					//pointlight._specular = glm::vec4(lightcomp->_diffuse, 1.f);

					// TODO
					//pointlight._radius = lightcomp->GetRadius();
				}
				else if (lightcomp->GetType() == Lights::SPOT)
				{
					SpotLight& spotlight = lightblock->_sLights[lightcomp->GetLightID()];
					spotlight._direction = transformComp->GetModelMatrix() * glm::vec4(1.f, 0.f, 0.f, 0.f); // w = 0 for vector
					spotlight.position = transformComp->GetModelMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f); // w = 1 for point
					spotlight._intensity = lightcomp->GetIntensity();
					//spotlight._ambient = glm::vec4(lightcomp->_ambient, 1.f);
					spotlight._diffuse = glm::vec4(lightcomp->GetColor(), 1.f);
					//spotlight._specular = glm::vec4(lightcomp->_diffuse, 1.f);
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

	//int LightSystem::AddDirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	//{
	//	// Locate available slot for light
	//	int id = 0;

	//	for (; id < s_MaxLights; ++id)
	//	{
	//		if (dLights_tracker[id] == false)
	//			break;
	//	}

	//	// Not enough available lights, return invalid id
	//	if (id >= s_MaxLights)
	//		return -1;

	//	lightblock->_dLights[id]._direction = glm::vec4(direction, 0.f);
	//	lightblock->_dLights[id]._ambient = glm::vec4(ambient, 1.f);
	//	lightblock->_dLights[id]._diffuse = glm::vec4(diffuse, 1.f);
	//	lightblock->_dLights[id]._specular = glm::vec4(specular, 1.f);

	//	// Set tracker
	//	dLights_tracker[id] = true;

	//	lightblock->_dLights_Num++;
	//	return id;
	//}

	//int LightSystem::AddPointLight(const float& intensity, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	//{
	//	// Locate available slot for light
	//	int id = 0;

	//	for (; id < s_MaxLights; ++id)
	//	{
	//		if (pLights_tracker[id] == false)
	//			break;
	//	}

	//	// Not enough available lights, return invalid id
	//	if (id >= s_MaxLights)
	//		return -1;

	//	lightblock->_pLights[id]._attenuation = 1.f / intensity;
	//	lightblock->_pLights[id]._ambient = glm::vec4(ambient, 1.f);
	//	lightblock->_pLights[id]._diffuse = glm::vec4(diffuse, 1.f);
	//	lightblock->_pLights[id]._specular = glm::vec4(specular, 1.f);

	//	// Set tracker
	//	pLights_tracker[id] = true;

	//	lightblock->_pLights_Num++;
	//	return id;
	//}

	//int LightSystem::AddSpotLight(const glm::vec3& direction, const float& cutoff, const float& outercutoff,
	//								const float& intensity, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	//{
	//	// Locate available slot for light
	//	int id = 0;

	//	for (; id < s_MaxLights; ++id)
	//	{
	//		if (sLights_tracker[id] == false)
	//			break;
	//	}

	//	// Not enough available lights, return invalid id
	//	if (id >= s_MaxLights)
	//		return -1;

	//	lightblock->_sLights[id]._direction = glm::vec4(direction, 0.f);
	//	lightblock->_sLights[id]._cutOff = cutoff;
	//	lightblock->_sLights[id]._outerCutOff = outercutoff;
	//	lightblock->_sLights[id]._attenuation = 1.f / intensity;
	//	lightblock->_sLights[id]._ambient = glm::vec4(ambient, 1.f);
	//	lightblock->_sLights[id]._diffuse = glm::vec4(diffuse, 1.f);
	//	lightblock->_sLights[id]._specular = glm::vec4(specular, 1.f);

	//	// Set tracker
	//	sLights_tracker[id] = true;

	//	lightblock->_sLights_Num++;
	//	return id;
	//}
	//void LightSystem::AttachLightComponent(Entity& entity, Lights lightType)
	//{
	//	// Ensure that previous light is deleted before assigning new light type
	//	switch (lightType)
	//	{
	//	case Lights::DIRECTIONAL:
	//		if (entity.getComponent<ComponentLight>() == nullptr)
	//			entity.AttachComponent<ComponentLight>(ComponentLight(AddDirLight(), Lights::DIRECTIONAL));
	//			
	//		break;
	//	case Lights::POINT:
	//		if (entity.getComponent<ComponentLight>() == nullptr)
	//			entity.AttachComponent<ComponentLight>(ComponentLight(AddPointLight(), Lights::POINT));
	//		break;
	//	case Lights::SPOT:
	//		if (entity.getComponent<ComponentLight>() == nullptr)
	//			entity.AttachComponent<ComponentLight>(ComponentLight(AddSpotLight(), Lights::SPOT));
	//		break;
	//	default:
	//		if (entity.getComponent<ComponentLight>() == nullptr)
	//			entity.AttachComponent<ComponentLight>(ComponentLight(AddDirLight(), Lights::DIRECTIONAL));
	//		break;
	//	}
	//}

	//void LightSystem::DetachLightComponent(Entity& entity)
	//{
	//	ComponentLight* lightcomponent = entity.getComponent<ComponentLight>();

	//	if (lightcomponent != nullptr)
	//	{
	//		// Delete current light from local data
	//		// Locate light in light tracker and set to false
	//		if (lightcomponent->_lightID != -1)
	//		{
	//			switch (lightcomponent->_type)
	//			{
	//			case Lights::DIRECTIONAL:
	//				if (lightcomponent->_lightID < lightblock->_dLights_Num - 1)
	//					SortLights(Lights::DIRECTIONAL, lightcomponent->_lightID);
	//				else
	//					dLights_tracker[lightcomponent->_lightID] = false;

	//				lightcomponent->_lightID = -1;
	//				break;

	//			case Lights::POINT:
	//				if (lightcomponent->_lightID < lightblock->_pLights_Num - 1)
	//					SortLights(Lights::POINT, lightcomponent->_lightID);
	//				else
	//					pLights_tracker[lightcomponent->_lightID] = false;

	//				lightcomponent->_lightID = -1;
	//				break;
	//			case Lights::SPOT:
	//				if (lightcomponent->_lightID < lightblock->_sLights_Num - 1)
	//					SortLights(Lights::SPOT, lightcomponent->_lightID);
	//				else
	//					sLights_tracker[lightcomponent->_lightID] = false;

	//				lightcomponent->_lightID = -1;
	//				break;
	//			default:
	//				break;
	//			}
	//		}
	//		// Delete component
	//		entity.RemoveComponent<ComponentLight>();
	//	}
	//}

	/*void LightSystem::RemoveLight(const int& id, Lights lightType)
	{
		if (id != -1)
		{
			switch (lightType)
			{
			case Lights::DIRECTIONAL:
				if (id < lightblock->_dLights_Num - 1)
					SortLights(Lights::DIRECTIONAL, id);
				else
				{
					dLights_tracker[id] = false;
					lightblock->_dLights_Num--;
				}
					
				break;

			case Lights::POINT:
				if (id < lightblock->_pLights_Num - 1)
					SortLights(Lights::POINT, id);
				else
				{
					pLights_tracker[id] = false;
					lightblock->_pLights_Num--;
				}
					
				break;
			case Lights::SPOT:
				if (id < lightblock->_sLights_Num - 1)
					SortLights(Lights::SPOT, id);
				else
				{
					sLights_tracker[id] = false;
					lightblock->_sLights_Num--;
				}
					
				break;
			default:
				break;
			}
		}
	}*/

	void LightSystem::SortLights(const Lights& lightType, const int& deletedIndex)
	{
		auto itr = G_ECMANAGER->begin<ComponentLight>();
		auto itrEnd = G_ECMANAGER->end<ComponentLight>();

		if (lightType == Lights::DIRECTIONAL)
		{
			// Decrement all valid light component IDs more than deleted index by 1 to pack tightly
			while (itr != itrEnd)
			{
				ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

				if (lightcomp->GetLightID() != -1 && lightcomp->GetType() == Lights::DIRECTIONAL && lightcomp->GetLightID() > deletedIndex)
				{
					//lightcomp->_lightID--;
					lightcomp->SetLightID(lightcomp->GetLightID() - 1);
				}
				++itr;
			}

			// Shift all directional lights in array by 1 to the left
			for (int i = deletedIndex + 1; i < lightblock->_dLights_Num; ++i)
				lightblock->_dLights[i - 1] = lightblock->_dLights[i];


			lightblock->_dLights_Num--; // decrement total number of lights
			//dLights_tracker[lightblock->_dLights_Num] = false;
		}
		else if (lightType == Lights::POINT)
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

			//pLights_tracker[lightblock->_pLights_Num] = false;
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

			//sLights_tracker[lightblock->_sLights_Num] = false;
		}
	}

//	void LightSystem::ChangeLightType(ComponentLight* light, Lights lightType)
//	{
//		if (light->_type == lightType)
//			return;
//
//		if (light != nullptr)
//		{
//			// Delete current light from local data
//			// Locate light in light tracker and set to false
//			if (light->_lightID != -1)
//			{
//				switch (light->_type)
//				{
//				case Lights::DIRECTIONAL:
//					if (light->_lightID < lightblock->_dLights_Num - 1)
//						SortLights(Lights::DIRECTIONAL, light->_lightID);
//					else
//					{
//						dLights_tracker[light->_lightID] = false;
//
//						lightblock->_dLights_Num--;
//					}
//					break;
//
//				case Lights::POINT:
//					if (light->_lightID < lightblock->_pLights_Num - 1)
//						SortLights(Lights::POINT, light->_lightID);
//					else
//					{
//						pLights_tracker[light->_lightID] = false;
//						lightblock->_pLights_Num--;
//					}
//						
//					break;
//				case Lights::SPOT:
//					if (light->_lightID < lightblock->_sLights_Num - 1)
//						SortLights(Lights::SPOT, light->_lightID);
//					else
//					{
//						sLights_tracker[light->_lightID] = false;
//						lightblock->_sLights_Num--;
//					}
//						
//					break;
//				default:
//					break;
//				}
//			}
//
//			switch (lightType)
//			{
//			case Lights::DIRECTIONAL:
//				light->AssignLight(AddDirLight(glm::vec3(1.f,0.f,0.f), light->_ambient, light->_diffuse, light->_specular), Lights::DIRECTIONAL);
//				light->_isActive = true;
//				break;
//			case Lights::POINT:
//				light->AssignLight(AddPointLight(light->_intensity, light->_ambient, light->_diffuse, light->_specular), Lights::POINT);
//				light->_isActive = true;
//				break;
//			case Lights::SPOT:
//				light->AssignLight(AddSpotLight(glm::vec3(1.f, 0.f, 0.f), light->_cutOff, light->_outerCutOff, light->_intensity, light->_ambient,
//												light->_diffuse, light->_specular), Lights::SPOT);
//				light->_isActive = true;
//				break;
//
//			case Lights::INVALID_TYPE:
//				light->AssignLight(-1, Lights::INVALID_TYPE);
//				light->_isActive = false;
//				break;
//
//			default:
//				break;
//			}
//
////#ifdef _DEBUG
////			if (light->_lightID == -1)
////				std::cout << "ERROR: Failed to create light component, please check Light System" << std::endl;
////#endif
//		}
//	}

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

	DirLight& LightSystem::GetDirLight(const int& id)
	{
		return lightblock->_dLights[id];
	}
	SpotLight& LightSystem::GetSpotLight(const int& id)
	{
		return lightblock->_sLights[id];
	}
	PointLight& LightSystem::GetPointLight(const int& id)
	{
		return lightblock->_pLights[id];
	}
	void LightSystem::SetLight(const Lights& type, ComponentLight* const& comp)
	{
		switch (type)
		{
		case Lights::DIRECTIONAL:
			SetDirectionalLight(comp);
			break;

		case Lights::POINT:
			SetPointLight(comp);
			break;

		case Lights::SPOT:
			SetSpotLight(comp);
			break;

		case Lights::INVALID_TYPE:
			RemoveLight(comp);
			break;
		}
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

	void LightSystem::SetColor(ComponentLight* const& comp, const glm::vec3& color)
	{
		// Check if comp has a valid light
		if (comp->GetLightID() == -1)
			return;

		switch (comp->GetType())
		{
		case Lights::DIRECTIONAL:
			lightblock->_dLights[comp->GetLightID()]._diffuse = glm::vec4(color, 1.f);
			break;
		case Lights::POINT:
			lightblock->_pLights[comp->GetLightID()]._diffuse = glm::vec4(color, 1.f);
			break;
		case Lights::SPOT:
			lightblock->_sLights[comp->GetLightID()]._diffuse = glm::vec4(color, 1.f);
			break;
		default:
			break;
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
}