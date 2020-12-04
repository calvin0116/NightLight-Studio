//#include "ComponentCScript.h"
//#include "../Logic/CScripts/AllScripts.h"
//
////#include "Components.h"   // inherit required functions
//int ComponentCScript::_blkCtrl = 0;
//
//ComponentCScript::ComponentCScript() : _isActive(false), _pScript(nullptr), _iTag(0)
//{
//    strcpy_s(ser_name, "CScriptComponent");
//    ++_blkCtrl;
//}
//
//ComponentCScript::~ComponentCScript()
//{
//  --_blkCtrl;
//  if (_blkCtrl > 0)
//    _pScript = nullptr;
//  // Delete memory
//  else
//  {
//    if (_pScript)
//      delete _pScript;
//    _pScript = nullptr;
//  }
//}
//
//ComponentCScript::ComponentCScript(const ComponentCScript& rhs) : _isActive(false), _pScript(nullptr), _iTag(0)
//{
//  *this = rhs;
//  ++_blkCtrl;
//}
//
//ComponentCScript& ComponentCScript::operator=(const ComponentCScript& rhs)
//{
//  *this = rhs;
//  ++_blkCtrl;
//  return *this;
//}
//
//void	ComponentCScript::Read(Value& val)
//{
//  if (val.FindMember("isActive") == val.MemberEnd())
//    std::cout << "No isActive data has been found" << std::endl;
//  else
//    _isActive = val["isActive"].GetBool();
//
//  if (val.FindMember("Name") == val.MemberEnd())
//    std::cout << "No Name data has been found" << std::endl;
//  else
//  {
//    _sName = val["Name"].GetString();
//    if (!_sName.empty())
//    {
//      if (_pScript) // Already has a script
//      {
//        delete _pScript;
//        _pScript = nullptr;
//      }
//      _pScript = AllScripts::MyConstruct(_sName.toString());
//    }
//  }
//
//  if (val.FindMember("Tag") == val.MemberEnd())
//    std::cout << "No tag data has been found" << std::endl;
//  else
//    _iTag = val["Tag"].GetInt();
//}
//
//Value	ComponentCScript::Write()
//{
//  Value val(rapidjson::kObjectType);
//  NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
//  NS_SERIALISER::ChangeData(&val, "Name", rapidjson::StringRef(_sName.c_str())); // Name
//  NS_SERIALISER::ChangeData(&val, "Tag", _iTag); // Tag
//  return val;
//}