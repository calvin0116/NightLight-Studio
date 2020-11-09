#include "ComponentScript.h"

//#include "Components.h"   // inherit required functions

ComponentScript::ComponentScript() : _isActive(true), _isRunning(true), _ScriptName(), _MonoData()
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

    if (val.FindMember("isRunning") == val.MemberEnd())
      std::cout << "No isRunning data has been found" << std::endl;
    else
      _isRunning = val["isRunning"].GetBool();

    if (val.FindMember("ScriptName") == val.MemberEnd())
      std::cout << "No ScriptName data has been found" << std::endl;
    else
      _ScriptName = val["ScriptName"].GetString();
	}
}

inline Value ComponentScript::Write() {

  Value val(rapidjson::kObjectType);
  NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
  NS_SERIALISER::ChangeData(&val, "isRunning", _isRunning);		//Bool
  NS_SERIALISER::ChangeData(&val, "ScriptName", rapidjson::StringRef(_ScriptName.c_str()));		//String

  return val;
}
