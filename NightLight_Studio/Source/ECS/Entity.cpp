/***************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : Entity.cpp
Author : Eric Aw Yuh Liang (100%)
DP email : yuhliangeric.a@digipen.edu
Course : GAM200F19-A
Brief Description :
		Contains data members and functionalities pertaining an Entity,
		as part of ECS.
**************************************************************************/
#include "Entity.h"
#include "TransformComponent.h"

/*
#include "Components.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include "TransformManager.h"
#include "AiManager.h"
*/
Entity::Entity()
	: ID(0), is_active(true), name("")//, layer(Layer("Default", 0))
{
	AddComponent<TransformComponent>();
}


Entity::Entity(const Entity& rhs)
	: ID(0), is_active(rhs.is_active), name(rhs.name), //layer(rhs.layer),
	component_vector(rhs.GetComponentList()), component_bitset(rhs.GetComponentBitSet())
{
	for (size_t i = 0; i < component_bitset.size(); ++i)
	{
		if (component_bitset[i])
		{
			for (size_t k = 0; k < component_vector.size(); ++k)
			{
				if (rhs.component_array[i]->GetComponentID() == component_vector[k]->GetComponentID())
				{
					component_array[i] = component_vector.at(k);
					component_array[i]->SetOwner(this);
					break;
				}
			}
		}
	}
}


Entity::~Entity()
{
}


Entity& Entity::operator=(const Entity& rhs)
{
	if (this == &rhs)
		return *this;

	is_active = rhs.GetActiveStatus();
	name = rhs.GetName();

	component_vector = rhs.GetComponentList();
	component_bitset = rhs.GetComponentBitSet();

	for (size_t i = 0; i < component_bitset.size(); ++i)
	{
		if (component_bitset[i])
		{
			for (size_t k = 0; k < component_vector.size(); ++k)
			{
				if (rhs.component_array[i]->GetComponentID() == component_vector[k]->GetComponentID())
				{
					component_array[i] = component_vector.at(k);
					component_array[i]->SetOwner(this);
					break;
				}
			}
		}
	}

	return *this;
}


void Entity::AddTag(std::string tag_name)
{
	if (tag_name == std::string())
		return;

	tags.emplace(tag_name);

	// Update the Entity Manager's tag map
	//EntityManager::GetInstance()->AddEntityToTagMap(tag_name, this);
}


void Entity::RemoveTag(std::string tag_name)
{
	if (tag_name == std::string())
		return;

	tags.erase(tag_name);

	// Update the Entity Manager's tag map
	//EntityManager::GetInstance()->RemoveEntityFromTagMap(tag_name, this);
}


std::set<std::string>& Entity::GetTagList()
{
	return tags;
}


bool Entity::HasEmptyTagList()
{
	return tags.empty();
}

/*
Layer Entity::GetLayer()
{
	return layer;
}


void Entity::SetLayer(const Layer& new_layer)
{
	layer = new_layer;
}


void Entity::SetLayer(std::string new_layer_name, int new_layer_pos)
{
	layer = Layer(new_layer_name, new_layer_pos);
}
*/

bool Entity::GetActiveStatus() const
{
	return is_active;
}

bool& Entity::GetActiveStatusRef()
{
	return is_active;
}


void Entity::SetActiveStatus(bool set_active)
{
	is_active = set_active;
}


bool Entity::GetPickableStatus() const
{
	return is_pickable;
}


bool& Entity::GetPickableStatusRef()
{
	return is_pickable;
}


void Entity::SetPickableStatus(bool state)
{
	is_pickable = state;
}


uint16_t Entity::GetID() const
{
	return ID;
}

uint16_t& Entity::GetIDRef()
{
  return ID;
}

void Entity::SetID(uint16_t set_ID)
{
	ID = set_ID;
}


std::string Entity::GetName() const
{
	return name;
}

std::string& Entity::GetNameRef()
{
  return name;
}


void Entity::SetName(std::string str)
{
	name = str;
}


ComponentVector Entity::GetComponentList() const
{
	return component_vector;
}


ComponentArray Entity::GetComponentArray() const
{
	return component_array;
}


std::bitset<32> Entity::GetComponentBitSet() const
{
	return component_bitset;
}


void Entity::DestroyComponents()
{
	for (auto& c : component_vector)
	{
		if (c)
		{
			/*
			if (c->GetOwner() && c->GetOwner()->HasComponent<Tree::BehaviourTree>() )
			{
				AiManager::GetInstance()->RemoveBt(&c->GetOwner()->GetComponent<Tree::BehaviourTree>());
			}
			if (c->GetOwner() && c->GetOwner()->HasComponent<AiComponent>())
			{
				AiManager::GetInstance()->RemoveAi(&c->GetOwner()->GetComponent<AiComponent>());
			}

			if (c->GetOwner() && c->GetOwner()->HasComponent<Rigidbody>())
			{
				PhysicsManager::GetInstance()->RemoveRigidBody(&c->GetOwner()->GetComponent<Rigidbody>());
			}

			if (c->GetOwner() && c->GetOwner()->HasComponent<ColliderComponent>())
			{
				CollisionManager::GetInstance()->RemoveColliderComponent(&c->GetOwner()->GetComponent<ColliderComponent>());
			}

			/*if (c->GetOwner() && c->GetOwner()->HasComponent<AnimationComponent>())
			{
				AnimationManager::GetInstance()->RemoveAnimationComponent(&c->GetOwner()->GetComponent<AnimationComponent>());
			}*/

			/*if (c->GetOwner() && c->GetOwner()->HasComponent<TransformComponent>())
			{
				TransformManager::GetInstance()->RemoveTransform(&c->GetOwner()->GetComponent<TransformComponent>());
			}*/
		}
		delete c;
		c = nullptr;
	};

	component_vector.erase(component_vector.begin(), component_vector.end());
}


void Entity::Free()
{
	DestroyComponents();
}


std::ostream& operator<<(std::ostream& os, const Entity& ent)
{
	os << "Entity(" << ent.name << ")"; //<< std::endl;
	return os;
}


/*
std::ostream Entity::Serialize(const std::ostream& os)
{
	os << name << std::endl;
	for (auto& c : component_vector)
	{
		c.Serialise(os);
	}
}*/

