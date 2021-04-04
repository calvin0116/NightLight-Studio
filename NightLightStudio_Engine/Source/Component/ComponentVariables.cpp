#include "ComponentVariables.h"
#include "Components.h"

ComponentVariables::ComponentVariables()
{
	strcpy_s(ser_name,"ComponentVariables");
}

ComponentVariables::~ComponentVariables()
{
}

inline void ComponentVariables::Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
{
	g_ecman;
	for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
	{
		if (itr->name == "IntList")
		{
			auto int_list_val = itr->value.GetArray();
			if(int_list.size() == 0)
				for (unsigned i = 0; i < int_list_val.Size(); ++i)
					int_list.push_back(int_list_val[i].GetInt());
			else
				for (unsigned i = 0; i < int_list_val.Size(); ++i)
					int_list.at(i) = int_list_val[i].GetInt();
		}
		else if(itr->name == "FloatList")
		{
			auto float_list_val = itr->value.GetArray();
			
			if (float_list.size() == 0)
				for (unsigned i = 0; i < float_list_val.Size(); ++i)
					float_list.push_back(float_list_val[i].GetFloat());
			else
				for (unsigned i = 0; i < float_list_val.Size(); ++i)
					float_list.at(i) = float_list_val[i].GetFloat();
		}
		else if (itr->name == "StringList")
		{
			auto string_list_val = itr->value.GetArray();
			
			if (string_list.size() == 0)
				for (unsigned i = 0; i < string_list_val.Size(); ++i)
					string_list.push_back(LocalString(string_list_val[i].GetString()) );		
			else
				for (unsigned i = 0; i < string_list_val.Size(); ++i)
					string_list.at(i) = LocalString(string_list_val[i].GetString());
		}
	}
}

inline Value ComponentVariables::Write()
{
	Value val(rapidjson::kObjectType);

	Value int_list_val(rapidjson::kArrayType);
	for(int & i : int_list)
		int_list_val.PushBack(i, global_alloc);
	NS_SERIALISER::ChangeData(&val, "IntList", int_list_val);

	Value string_list_val(rapidjson::kArrayType);
	for (LocalString<DEF_STR_SIZE>& s : string_list)
		string_list_val.PushBack(rapidjson::StringRef(s.c_str()), global_alloc);
	NS_SERIALISER::ChangeData(&val, "StringList", string_list_val);

	Value float_list_val(rapidjson::kArrayType);
	for (float& f : float_list)
		float_list_val.PushBack(f, global_alloc);
	NS_SERIALISER::ChangeData(&val, "FloatList", float_list_val);


  return val;
}

