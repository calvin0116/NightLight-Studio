/***************************************************************************
Brief Description :
		Contains data members and functionalities pertaining an Entity,
		as part of ECS.
**************************************************************************/
#pragma once
//#include "stdafx.h"
#include <set>
#include <array>
#include <bitset>
#include "Component.h"
//#include "Layer.h"


class Component;


/*
	When a new Component ID is requested, the static ID (behaves
	like a counter) is incremented and returned.
*/
inline uint16_t GetNewComponentID()
{
	static uint16_t component_ID = 0;
	return component_ID++;
}


/*
	Every instantiation of a new type of Component will
	result in a new ID being created, and this ID will be
	used for all instances of that particular component.
*/
template <class T>
uint16_t GetNewComponentID()
{
	static uint16_t typeID = GetNewComponentID();
	return typeID;
}


class ComponentVector : public std::vector<Component*>
{
public:
	ComponentVector() = default;

	ComponentVector(const ComponentVector& rhs)
		:std::vector<Component*>()
	{
		for (uint16_t i = 0; i < rhs.size(); ++i)
			emplace_back(rhs.at(i)->Clone());
	}

	ComponentVector& operator= (const ComponentVector& rhs)
	{
		if (this == &rhs)
			return *this;

		for (int i = 0; i < rhs.size(); ++i)
			emplace_back(rhs.at(i)->Clone());

		return *this;
	}
};


class ComponentArray : public std::array<Component*, 32>
{
public:
	ComponentArray() = default;
};


/*
	An Entity has a unique ID, and also contains a flag which
	stores its state of being active.
*/
class Entity
{
private:
	uint16_t ID;
	std::string name;

	bool is_active;
	bool is_pickable = true;

	//Layer layer;

	ComponentVector component_vector;
	ComponentArray component_array;
	std::bitset<32> component_bitset;

	std::set<std::string> tags;
	
public:
	Entity();
	Entity(const Entity& rhs);
	~Entity();

	Entity& operator= (const Entity& rhs);

	// Tagging system
	void AddTag(std::string tag_name);
	void RemoveTag(std::string tag_name);
	std::set<std::string>& GetTagList();
	bool HasEmptyTagList();

	/*
	// Layer system
	Layer GetLayer();
	void SetLayer(const Layer& new_layer);
	void SetLayer(std::string new_layer_name, int new_layer_pos);
	*/
	/* Getter and Setter for the Entity's active status */
	bool GetActiveStatus() const;
	bool& GetActiveStatusRef();
	void SetActiveStatus(bool set_active);

	bool GetPickableStatus() const;
	bool& GetPickableStatusRef();
	void SetPickableStatus(bool state);

	/* Getter and Setter for the Entity's ID */
	uint16_t GetID() const;
	uint16_t& GetIDRef();
	void SetID(uint16_t set_ID);

	/* Getter and Setter for the Entity's Name */
	std::string GetName() const;
  std::string& GetNameRef();
	void SetName(std::string);

	void DestroyComponents();
	void Free();

	ComponentVector GetComponentList() const;
	ComponentArray GetComponentArray() const;
	std::bitset<32> GetComponentBitSet() const;

	// Add a Component to the Entity
	template <class T, typename... Args>
	T& AddComponent(Args... args);

	// Add a Component to the Entity (Run time)
	template <class T, typename... Args>
	T& AddComponentWithInit(Args ...args);
	// Add component as another component to the entity
	template <class T, class T2, typename... Args>
	T& AddComponentAs(Args... args);

	// Add component as another component to the entity (Run time)
	template <class T, class T2, typename... Args>
	T& AddComponentAsWithInit(Args ...args);

	// Access a specific Component within the Entity
	template <class T>
	T& GetComponent() const;

	// Remove a Component from the Entity
	template <class T>
	void RemoveComponent();

	template <class T>
	void NullifyComponent();

	// Remove a Component from the Entity

	// Takes in a Component type with template class
	// Returns a true/false if the component exists
	// Could be used inside GetComponent<T>() for exception catching
	template <class T>
	bool HasComponent() const;

	friend std::ostream& operator<<(std::ostream& , const Entity&);
};


#include "Entity.hpp"
