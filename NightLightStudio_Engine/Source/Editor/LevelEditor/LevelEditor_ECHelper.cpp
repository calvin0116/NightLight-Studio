#include "LevelEditor_ECHelper.h"
#include "../../Component/Components.h"


namespace LS
{
	size_t findCaseInsensitive(std::string data, std::string toSearch, size_t pos)
	{
		std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) {return (unsigned char)::tolower(c); });
		std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), [](unsigned char c) {return (unsigned char)::tolower(c); });
		return data.find(toSearch, pos);
	}

	void ReadIntoCompVec(Entity ent, ENTITY_COMP_DATA* data, bool leftTransOut)
	{
		ENTITY_COMP_DATA& compData = *data;
		if (!leftTransOut && ent.getComponent<ComponentTransform>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<ComponentTransform>()->Write());
			compData.insert(std::make_pair(typeid(ComponentTransform).hash_code(), comp));
		}
		if (ent.getComponent<ComponentLoadAudio>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<ComponentLoadAudio>()->Write());
			compData.insert(std::make_pair(typeid(ComponentLoadAudio).hash_code(), comp));
		}
		if (ent.getComponent<ComponentGraphics>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<ComponentGraphics>()->Write());
			compData.insert(std::make_pair(typeid(ComponentGraphics).hash_code(), comp));
		}
		if (ent.getComponent<ColliderComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<ColliderComponent>()->Write());
			compData.insert(std::make_pair(typeid(ColliderComponent).hash_code(), comp));
		}
		if (ent.getComponent<RigidBody>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<RigidBody>()->Write());
			compData.insert(std::make_pair(typeid(RigidBody).hash_code(), comp));
		}
		if (ent.getComponent<LightComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<LightComponent>()->Write());
			//ent.getComponent<LightComponent>()->SetType(NS_GRAPHICS::Lights::INVALID_TYPE);
			compData.insert(std::make_pair(typeid(LightComponent).hash_code(), comp));
		}
		if (ent.getComponent<ScriptComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<ScriptComponent>()->Write());
			compData.insert(std::make_pair(typeid(ScriptComponent).hash_code(), comp));
		}
		if (ent.getComponent<CanvasComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<CanvasComponent>()->Write());
			compData.insert(std::make_pair(typeid(CanvasComponent).hash_code(), comp));
		}
		if (!leftTransOut && ent.getComponent<CScriptComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<CScriptComponent>()->Write());
			compData.insert(std::make_pair(typeid(CScriptComponent).hash_code(), comp));
		}
		if (ent.getComponent<PlayerStatsComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<PlayerStatsComponent>()->Write());
			compData.insert(std::make_pair(typeid(PlayerStatsComponent).hash_code(), comp));
		}
		if (ent.getComponent<CauldronStatsComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<CauldronStatsComponent>()->Write());
			compData.insert(std::make_pair(typeid(CauldronStatsComponent).hash_code(), comp));
		}
		if (!leftTransOut && ent.getComponent<VariablesComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<VariablesComponent>()->Write());
			compData.insert(std::make_pair(typeid(VariablesComponent).hash_code(), comp));
		}
		if (!leftTransOut && ent.getComponent<NavComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<NavComponent>()->Write());
			compData.insert(std::make_pair(typeid(NavComponent).hash_code(), comp));
		}
		if (ent.getComponent<AnimationComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<AnimationComponent>()->Write());
			compData.insert(std::make_pair(typeid(AnimationComponent).hash_code(), comp));
		}
		if (ent.getComponent<WayPointMapComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<WayPointMapComponent>()->Write());
			compData.insert(std::make_pair(typeid(WayPointMapComponent).hash_code(), comp));
		}
		if (ent.getComponent<WayPointComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<WayPointComponent>()->Write());
			compData.insert(std::make_pair(typeid(WayPointComponent).hash_code(), comp));
		}
		if (ent.getComponent<EmitterComponent>())
		{
			ENTITY_COMP_INFO comp(ent.getComponent<EmitterComponent>()->Write());
			//NS_GRAPHICS::EmitterSystem::GetInstance().RemoveEmitterByID(ent.getComponent<EmitterComponent>()->_emitterID);
			compData.insert(std::make_pair(typeid(EmitterComponent).hash_code(), comp));
		}
	}
}