#include "ComponentScript.h"

//#include "Components.h"   // inherit required functions

ComponentScript::ComponentScript()
{
	strcpy_s(ser_name,"ScriptComponent");
}

ComponentScript::~ComponentScript()
{

}


//read and write function for initialization from saved files

inline void ComponentScript::Read(Value& val) 
{ 
	for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
	{
		if (itr->name == "isActive")	//bool
			_isActive = itr->value.GetBool();

		if (itr->name == "Script")		//Vector of object
		{
			const Value& script = itr->value;

			data d;
			d._isActive = script["isActive"].GetBool();
			d._isRunning = script["isRunning"].GetBool();
			strcpy_s(d._ScriptName , script["Script Name"].GetString());

			_Scripts.push_back(d);
		}
	}

}

inline Value ComponentScript::Write() {
	Value val(rapidjson::kObjectType);
	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	Value all_script(rapidjson::kObjectType);
	for (int i = 0; i < _Scripts.size(); ++i)
	{
		data d = _Scripts.at(i);
		Value script(rapidjson::kObjectType);

		NS_SERIALISER::ChangeData(&script, "isActive", d._isActive);		//Bool
		NS_SERIALISER::ChangeData(&script, "isRunning", d._isRunning);
		std::string sn = d._ScriptName;
		NS_SERIALISER::ChangeData(&script, "Script Name", rapidjson::StringRef(sn.c_str()));

		NS_SERIALISER::ChangeData(&all_script, "Script", script);
	}
	NS_SERIALISER::ChangeData(&val, "Scripts", all_script);

	return val;

}
