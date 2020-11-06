#include "ComponentCScript.h"

//#include "Components.h"   // inherit required functions

ComponentCScript::ComponentCScript() : _isActive(false)
{

}

ComponentCScript::~ComponentCScript()
{

}

void	ComponentCScript::Read(Value& val)
{
  if (val.FindMember("isActive") == val.MemberEnd())
    std::cout << "No active data has been found" << std::endl;
  else
    _isActive = val["isActive"].GetBool();

  if (val.FindMember("Texture file") == val.MemberEnd())
    std::cout << "No Texture file data has been found" << std::endl;
  else
    _sName = val["Texture file"].GetString();
}

Value	ComponentCScript::Write()
{
  Value val(rapidjson::kObjectType);
  NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
  NS_SERIALISER::ChangeData(&val, "Name", rapidjson::StringRef(_sName.c_str())); // Name
  return val;
}