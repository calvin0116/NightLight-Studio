/***************************************************************************
Brief Description :
			Json header files and related defines
**************************************************************************/
#pragma once
#include "JsonHeader.h"
#include "../../Component/ComponentManager.h"

namespace NS_SERIALISER {
    //!! Type T Must have ISerialisable and a Component 

    //~~~! Helper function that is not declared in class due to Entity not declared in .h
    inline void ComponentsCreation(Value& Comp_list, Entity& entity);
	ENGINE_API inline void EntityListCreation(Value& Ent_list);


	inline void ComponentsInit(Value& Comp_list, Entity& entity);
	ENGINE_API inline void EntityListInit(Value& Ent_list);
	//For object access change
	template<typename d_type>
	inline void ChangeData(Value* val, std::string d_name, d_type data)
	{
		Value key(d_name.c_str(), global_alloc);
		Value _data(data);
		val->GetObject().AddMember(key, _data, global_alloc);
	}

	inline void ChangeData(Value* val, std::string d_name, Value& data)
	{
		Value key(d_name.c_str(), global_alloc);
		//Value _data(data);
		val->GetObject().AddMember(key, data, global_alloc);
	}
}