#include "ComponentScript.h"
#include "../Mono/MonoWrapper.h"

//#include "Components.h"   // inherit required functions

ComponentScript::ComponentScript() : _isActive(true), _isRunning(true), _ScriptName(), _MonoData()
{
	strcpy_s(ser_name,"ScriptComponent");
}

ComponentScript::~ComponentScript()
{
  MonoWrapper::FreeGCHandle(_MonoData._GCHandle);
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
    
    // Create script instance, and set script saved values
    if (!_ScriptName.empty())
    {
      _MonoData._pInstance = MonoWrapper::ConstructObject(_ScriptName.toString());
      _MonoData._GCHandle = MonoWrapper::ConstructGCHandle(_MonoData._pInstance);
      int ID = G_ECMANAGER->getEntity(this).getId();
      MonoWrapper::SetObjectFieldValue(_MonoData._pInstance, "id", ID);

      // Get script values
      if (_MonoData._pInstance)
      {
        MonoClass* klass = MonoWrapper::GetMonoClass(_MonoData._pInstance);
        void* iter = NULL;
        MonoClassField* field = mono_class_get_fields(klass, &iter);
        
        while (field)
        {
          // Name of variables
          const char* var_name = mono_field_get_name(field);
          // Type id
          int var_typeid = mono_type_get_type(mono_field_get_type(field));
          // To check for public fields/variables
          unsigned var_flag = mono_field_get_flags(field);
          if (var_flag == MONO_FIELD_ATTR_PUBLIC) // MONO_FIELD_ATTR_PUBLIC
          {
            // Inspect values here
            // type of variable
            if (var_typeid == MONO_TYPE_BOOLEAN) // bool
            {
              //std::cout << "Bool" << std::endl;
              std::string tempVar(var_name);
              tempVar += "bool";
              if (val.FindMember(tempVar.c_str()) == val.MemberEnd())
                std::cout << "No script bool data has been found" << std::endl;
              else
              {
                bool monoVal = val[tempVar.c_str()].GetBool();
                MonoWrapper::SetObjectFieldValue(_MonoData._pInstance, var_name, monoVal);
              }
            }
            else if (var_typeid == MONO_TYPE_I4) // int
            {
              std::string tempVar(var_name);
              tempVar += "int";
              if (val.FindMember(tempVar.c_str()) == val.MemberEnd())
                std::cout << "No script int data has been found" << std::endl;
              else
              {
                int monoVal = val[tempVar.c_str()].GetInt();
                MonoWrapper::SetObjectFieldValue(_MonoData._pInstance, var_name, monoVal);
              }
            }
            else if (var_typeid == MONO_TYPE_U4) // unsigned
            {
              //std::cout << "Unsigned" << std::endl;
              std::string tempVar(var_name);
              tempVar += "unsigned";
              if (val.FindMember(tempVar.c_str()) == val.MemberEnd())
                std::cout << "No script unsigned data has been found" << std::endl;
              else
              {
                unsigned monoVal = val[tempVar.c_str()].GetUint();
                MonoWrapper::SetObjectFieldValue(_MonoData._pInstance, var_name, monoVal);
              }
            }
            else if (var_typeid == MONO_TYPE_R4) // float
            {
              //std::cout << "Float" << std::endl;
              std::string tempVar(var_name);
              tempVar += "float";
              if (val.FindMember(tempVar.c_str()) == val.MemberEnd())
                std::cout << "No script float data has been found" << std::endl;
              else
              {
                float monoVal = val[tempVar.c_str()].GetFloat();
                MonoWrapper::SetObjectFieldValue(_MonoData._pInstance, var_name, monoVal);
              }
            }
            else if (var_typeid == MONO_TYPE_R8) // double
            {
              //std::cout << "Double" << std::endl;
              std::string tempVar(var_name);
              tempVar += "double";
              if (val.FindMember(tempVar.c_str()) == val.MemberEnd())
                std::cout << "No script double data has been found" << std::endl;
              else
              {
                double monoVal = val[tempVar.c_str()].GetDouble();
                MonoWrapper::SetObjectFieldValue(_MonoData._pInstance, var_name, monoVal);
              }
            }
            else if (var_typeid == MONO_TYPE_STRING) // string
            {
              ////std::cout << "String" << std::endl;
              MonoString* monoString = MonoWrapper::GetObjectFieldValue<MonoString*>(_MonoData._pInstance, var_name);

              /**** For saving/getting ID ****/
              int i = 0;
              for (; i < _savedCount; ++i)
              {
                // Found ID
                std::string tempVar(var_name);
                std::string tempSaved(_SavedID[i]);
                if (tempVar == tempSaved)
                  break;
              }

              if (monoString != nullptr && _savedCount != 0)
              {
                std::string tempVar(var_name);
                tempVar += "string";
                if (val.FindMember(tempVar.c_str()) == val.MemberEnd())
                  std::cout << "No script string data has been found" << std::endl;
                else
                {
                  _SavedID[i] = val[tempVar.c_str()].GetString();
                  monoString = MonoWrapper::ToMonoString(_SavedID[i]);
                  MonoWrapper::SetObjectFieldValue(_MonoData._pInstance, var_name, *monoString);
                }
              }
            }
            else
            {
              std::cout << "Unsupported type!" << std::endl;
            }
          }
          // Move to next field
          field = mono_class_get_fields(klass, &iter);
        }
      }
    }
	}
}

inline Value ComponentScript::Write() {

  Value val(rapidjson::kObjectType);
  NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
  NS_SERIALISER::ChangeData(&val, "isRunning", _isRunning);		//Bool
  NS_SERIALISER::ChangeData(&val, "ScriptName", rapidjson::StringRef(_ScriptName.c_str()));		//String
  
  // Save script values
  if (_MonoData._pInstance)
  {
    MonoClass*      klass = MonoWrapper::GetMonoClass(_MonoData._pInstance);
    void*           iter = NULL;
    MonoClassField* field = mono_class_get_fields(klass, &iter);

    while (field)
    {
      // Name of variables
      const char* var_name = mono_field_get_name(field);
      // Type id
      int var_typeid = mono_type_get_type(mono_field_get_type(field));
      // To check for public fields/variables
      unsigned var_flag = mono_field_get_flags(field);
      if (var_flag == MONO_FIELD_ATTR_PUBLIC) // MONO_FIELD_ATTR_PUBLIC
      {
        // Inspect values here
        // type of variable
        if (var_typeid == MONO_TYPE_BOOLEAN) // bool
        {
          ////std::cout << "Bool" << std::endl;
          std::string tempVar(var_name);
          tempVar += "bool";
          NS_SERIALISER::ChangeData(&val, tempVar.c_str(), MonoWrapper::GetObjectFieldValue<bool>(_MonoData._pInstance, var_name));
        }
        else if (var_typeid == MONO_TYPE_I4) // int
        {
          //std::cout << "Int" << std::endl;
          std::string tempVar(var_name);
          tempVar += "int";
          NS_SERIALISER::ChangeData(&val, tempVar.c_str(), MonoWrapper::GetObjectFieldValue<int>(_MonoData._pInstance, var_name));
        }
        else if (var_typeid == MONO_TYPE_U4) // unsigned
        {
          ////std::cout << "Unsigned" << std::endl;
          std::string tempVar(var_name);
          tempVar += "unsigned";
          NS_SERIALISER::ChangeData(&val, tempVar.c_str(), MonoWrapper::GetObjectFieldValue<unsigned>(_MonoData._pInstance, var_name));
        }
        else if (var_typeid == MONO_TYPE_R4) // float
        {
          ////std::cout << "Float" << std::endl;
          std::string tempVar(var_name);
          tempVar += "float";
          NS_SERIALISER::ChangeData(&val, tempVar.c_str(), MonoWrapper::GetObjectFieldValue<float>(_MonoData._pInstance, var_name));
        }
        else if (var_typeid == MONO_TYPE_R8) // double
        {
          ////std::cout << "Double" << std::endl;
          std::string tempVar(var_name);
          tempVar += "double";
          NS_SERIALISER::ChangeData(&val, tempVar.c_str(), MonoWrapper::GetObjectFieldValue<double>(_MonoData._pInstance, var_name));
        }
        else if (var_typeid == MONO_TYPE_STRING) // string
        {
          //std::cout << "String" << std::endl;
          MonoString* monoString = MonoWrapper::GetObjectFieldValue<MonoString*>(_MonoData._pInstance, var_name);
          //std::string toWrite;
          /**** For saving/getting ID ****/
          int i = 0;
          std::string tempVar(var_name);
          tempVar += "string";
          for (; i < _savedCount; ++i)
          {
            // Found ID
            std::string tempSaved(_SavedID[i]);
            if (tempVar == tempSaved)
              break;
          }
          if (i == _savedCount)
          {
            _SavedID[i] = tempVar;
            ++_savedCount;
          }
          /************************/
          //std::string& saved = _SavedStrings[i];
          if (monoString != nullptr)
          {
            _SavedStrings[i] = MonoWrapper::ToString(monoString);
            NS_SERIALISER::ChangeData(&val, tempVar.c_str(), rapidjson::StringRef(_SavedStrings[i].c_str()));		//String
          }
        }
        else
        {
          std::cout << "Unsupported type!" << std::endl;
        }
      }
      // Move to next field
      field = mono_class_get_fields(klass, &iter);
    }
  }


  return val;
}
