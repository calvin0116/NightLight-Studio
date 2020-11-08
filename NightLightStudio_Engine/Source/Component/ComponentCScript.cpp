#include "ComponentCScript.h"

//#include "Components.h"   // inherit required functions

ComponentCScript::ComponentCScript() : _isActive(false), _iTag(0)
{
    strcpy_s(ser_name, "CScriptComponent");
}

ComponentCScript::~ComponentCScript()
{

}

void	ComponentCScript::Read(Value& val)
{
  if (val.FindMember("isActive") == val.MemberEnd())
    std::cout << "No isActive data has been found" << std::endl;
  else
    _isActive = val["isActive"].GetBool();

  if (val.FindMember("Name") == val.MemberEnd())
    std::cout << "No Name data has been found" << std::endl;
  else
    _sName = val["Name"].GetString();

  if (val.FindMember("Tag") == val.MemberEnd())
    std::cout << "No tag data has been found" << std::endl;
  else
    _iTag = val["Tag"].GetInt();
}

Value	ComponentCScript::Write()
{
  Value val(rapidjson::kObjectType);
  NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
  NS_SERIALISER::ChangeData(&val, "Name", rapidjson::StringRef(_sName.c_str())); // Name
  NS_SERIALISER::ChangeData(&val, "Tag", _iTag); // Tag
  return val;
}