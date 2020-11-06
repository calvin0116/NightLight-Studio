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
    if (val.FindMember("isActive") == val.MemberEnd())
      std::cout << "No isActive data has been found" << std::endl;
    else
      _isActive = val["isActive"].GetBool();

    if (val.FindMember("isActive1") == val.MemberEnd())
      std::cout << "No isActive1 data has been found" << std::endl;
    else
      _Script1._isActive = val["isActive1"].GetBool();

    if (val.FindMember("isRunning1") == val.MemberEnd())
      std::cout << "No isRunning1 data has been found" << std::endl;
    else
      _Script1._isRunning = val["isRunning1"].GetBool();

    if (val.FindMember("Name1") == val.MemberEnd())
      std::cout << "No Name1 data has been found" << std::endl;
    else
      _Script1._ScriptName = val["Name1"].GetString();

    if (val.FindMember("isActive2") == val.MemberEnd())
      std::cout << "No isActive2 data has been found" << std::endl;
    else
      _Script2._isActive = val["isActive2"].GetBool();

    if (val.FindMember("isRunning2") == val.MemberEnd())
      std::cout << "No isRunning2 data has been found" << std::endl;
    else
      _Script2._isRunning = val["isRunning2"].GetBool();

    if (val.FindMember("Name2") == val.MemberEnd())
      std::cout << "No Name2 data has been found" << std::endl;
    else
      _Script2._ScriptName = val["Name2"].GetString();

		//if (itr->name == "isActive")	//bool
		//	_isActive = itr->value.GetBool();

		//if (itr->name == "Scripts")		//Vector of object
		//{
		//	const Value& all_script = itr->value[itr->name];
		//	for (Value::ConstMemberIterator itr = all_script.MemberBegin(); itr != all_script.MemberEnd(); ++itr)
		//	{
		//		const Value& script = itr->value;
		//		
		//		data d;
		//		d._isActive = script["isActive"].GetBool();
		//		d._isRunning = script["isRunning"].GetBool();
		//		d._ScriptName = script["Script Name"].GetString();

		//		_Scripts.push_back(d);
		//	}
		//}
	}
}

inline Value ComponentScript::Write() {
	//Value val(rapidjson::kObjectType);
	//NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	//Value all_script(rapidjson::kObjectType);
	//for (int i = 0; i < _Scripts.size(); ++i)
	//{
	//	data d = _Scripts.at(i);
	//	Value script(rapidjson::kObjectType);

	//	NS_SERIALISER::ChangeData(&script, "isActive", d._isActive);		//Bool
	//	NS_SERIALISER::ChangeData(&script, "isRunning", d._isRunning);
	//	std::string sn = d._ScriptName;
	//	NS_SERIALISER::ChangeData(&script, "Script Name", rapidjson::StringRef(sn.c_str()));

	//	NS_SERIALISER::ChangeData(&all_script, "Script", script);
	//}
	//NS_SERIALISER::ChangeData(&val, "Scripts", all_script);

  Value val(rapidjson::kObjectType);
  NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

  NS_SERIALISER::ChangeData(&val, "isActive1", _Script1._isActive);		//Bool
  NS_SERIALISER::ChangeData(&val, "isRunning1", _Script1._isRunning);		//Bool
  NS_SERIALISER::ChangeData(&val, "Name1", rapidjson::StringRef(_Script1._ScriptName.c_str()));		//String

  NS_SERIALISER::ChangeData(&val, "isActive2", _Script2._isActive);		//Bool
  NS_SERIALISER::ChangeData(&val, "isRunning2", _Script2._isRunning);		//Bool
  NS_SERIALISER::ChangeData(&val, "Name2", rapidjson::StringRef(_Script2._ScriptName.c_str()));		//String

  return val;
}
