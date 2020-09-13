/***************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : Component.h
Author : Eric Aw Yuh Liang
DP email : yuhliangeric.a@digipen.edu
Course : GAM200F19-A
Brief Description :
	Contains all data members and member functions that are part of
	a Component, that will be attached to an Entity.
**************************************************************************/
#pragma once
//#include "SerializationManager.h"
//#include "nlohmann json/json.hpp"
#include "Header.h"

class Entity;

class Component
{
private:
	Entity* owner = nullptr;
	uint16_t component_ID = 0;
	bool is_updateable = true;

protected:
	std::string component_name = "";
	Component* copy = nullptr;

public:
	Component() {}
	Component(const Component& rhs) : component_ID(rhs.component_ID), is_updateable(rhs.is_updateable), component_name(rhs.component_name)
	{}
	virtual ~Component() {}

	virtual void Load() {}
	virtual void Init() {}
	virtual void Update() {}

	//virtual void
	/*
	//Json Functions
	virtual nlohmann::json Serialize() { return nullptr; } 
	virtual nlohmann::json SerializeOverWrite() { return Serialize(); };
	virtual void Deserialize(nlohmann::json prefab) {}
	virtual void DeserializeOverWrite(nlohmann::json obj) {
		//Do same as serialize since all the variable will be show
		//If you do not want, overwrite this function
		Deserialize(obj);
	}*/


	bool GetUpdateStatus();
	void SetUpdateStatus(bool status);

	uint16_t GetComponentID() const;
	void SetComponentID(uint16_t ID);

	void SetComponentName(std::string Name);

	std::string GetComponentName();

	Entity* GetOwner() const;
	void SetOwner(Entity* set_owner);

	//template <typename T>
	//T& GetSibling()
	//{
	//	return /**static_cast<T*>*/(owner->GetComponent<T>());
	//}

	/*template <typename T>
	bool HasSibling() const
	{
		return owner->HasComponent<T>();
	}*/

	virtual Component* Clone()
	{
		return new Component(*this);
	}

	//For copy usage
	void StoreCopy()
	{
		
		if (copy != nullptr)
			delete copy;

		copy = Clone();

	}

	virtual void RevertToCopy()
	{
		
		if (copy != nullptr)
		{
			*this = *copy;
			delete copy;
			copy = nullptr;
		}
	}
};

