#include "Entity.h"
/***************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : Entity.hpp
Author : Eric Aw Yuh Liang
DP email : yuhliangeric.a@digipen.edu
Course : GAM200F19-A
Brief Description :
		Contains data members and functionalities pertaining an Entity,
		as part of ECS.
**************************************************************************/

/*
	Adds/Constructs a new Component, and accepts the arguments to this
	function as the Component's constructor arguments.
*/
template<class T, typename ...Args>
T& Entity::AddComponent(Args ...args)
{
	T* c = new T(args...);
	c->SetOwner(this);
	c->SetComponentID(GetNewComponentID<T>());

	if (strcmp(typeid(*c).name(), "class ColliderComponent")==0)
	{
		//std::cout << "You have made it~!" << std::endl;
	}

	component_vector.emplace_back(c);
	component_array[GetNewComponentID<T>()] = c;
	component_bitset[GetNewComponentID<T>()] = true;
	
	//Use load rather then c->Init();
	c->Load();

	return *c;
}

//Used only for Runtime Addcomponent
template<class T, typename ...Args>
T& Entity::AddComponentWithInit(Args ...args)
{
	T* c = &AddComponent<T, Args...>(std::forward<Args>(args)...);
	c->Init();

	return *c;
}


template<class T, class T2, typename ...Args>
T& Entity::AddComponentAs(Args ...args)
{
	T* c = new T(args...);
	c->SetOwner(this);
	c->SetComponentID(GetNewComponentID<T>());

	if (strcmp(typeid(*c).name(), "class ColliderComponent") == 0)
	{
		//std::cout << "You have made it~!" << std::endl;
	}

	component_vector.emplace_back(c);
	component_array[GetNewComponentID<T>()] = c;
	component_bitset[GetNewComponentID<T>()] = true;
	component_array[GetNewComponentID<T2>()] = c;
	component_bitset[GetNewComponentID<T2>()] = true;
	c->Load();

	return *c;
}

template<class T, class T2, typename ...Args>
T& Entity::AddComponentAsWithInit(Args ...args)
{
	T* c = &AddComponentAs<T,T2, Args...>(std::forward<Args>(args)...);
	c->Init();

	return *c;
}

template<class T>
T& Entity::GetComponent() const
{
  	return *static_cast<T*>(component_array[GetNewComponentID<T>()]);
	/*
	if (this->HasComponent<T>())
		return *static_cast<T*>(component_array[GetNewComponentID<T>()]);
	else
	{
		std::ostringstream oss;
		oss << "Component " << typeid(T).name() << "does not exist";
		LOG_INFO(oss.str().c_str());
		//shouldn't happen
		return *static_cast<T*>(component_array[GetNewComponentID<T>()]);
	}*/

}


template<class T>
void Entity::RemoveComponent()
{
	component_array[GetNewComponentID<T>()] = nullptr;
	component_bitset[GetNewComponentID<T>()] = false;

	for (std::vector<Component*>::iterator c = component_vector.begin(); c != component_vector.end();)
	{
		if ((*c)->GetComponentID() == GetNewComponentID<T>())
		{
			delete (*c);
			(*c) = nullptr;
			c = component_vector.erase(c);
		}
		else
			++c;
	}
}

template<class T>
void Entity::NullifyComponent()
{
	component_array[GetNewComponentID<T>()] = nullptr;
	component_bitset[GetNewComponentID<T>()] = false;
}

template<class T>
bool Entity::HasComponent() const
{
	return component_bitset[GetNewComponentID<T>()];
}

