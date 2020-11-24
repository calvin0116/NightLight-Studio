#include "ComponentLoadAudio.h"

//#include "Components.h"   // inherit required functions

ComponentLoadAudio::ComponentLoadAudio()
{
	strcpy_s(ser_name, "AudioManager");
}

ComponentLoadAudio::~ComponentLoadAudio()
{

}

void	ComponentLoadAudio::Read(Value& val)
{
  if (val.FindMember("ListOfSound") == val.MemberEnd())
    std::cout << "No Force data has been found" << std::endl;
  else
  {
      if (val["ListOfSound"].IsObject())
      {
          auto sound_array = val["ListOfSound"].GetObject();
          bool empty_at_start = (_sounds.size() == 0);

          int index = 0;
          for (Value::ConstMemberIterator itr = sound_array.MemberBegin(); itr != sound_array.MemberEnd(); ++itr)
          {
              data MyData;
              strcpy_s(MyData.name, 128, itr->name.GetString());
              strcpy_s(MyData.path, 512, itr->value.GetString());
              if(empty_at_start)
                _sounds.push_back(MyData);
              else
                 _sounds.at(index) = MyData;
              ++index;
          }
      }
      //Should be removed if tested removal without issue
      else if (val["ListOfSound"].IsArray())
      {
          auto sound_array = val["ListOfSound"].GetArray();
          bool empty_at_start = (_sounds.size() == 0);


          for (int i = 0; i < sound_array.Size(); ++i)
          {
              fs::path cur_path_name = sound_array[i].GetString();
              data MyData;
              std::string MyName = cur_path_name.stem().string();
              std::string MyPath = cur_path_name.parent_path().string() + "/" + MyName + cur_path_name.extension().string();
              strcpy_s(MyData.name, 128, MyName.c_str());
              strcpy_s(MyData.path, 512, MyPath.c_str());
              if (empty_at_start)
                  _sounds.push_back(MyData);
              else
                  _sounds.at(i) = MyData;
          }
      }
  }
}

inline Value ComponentLoadAudio::Write() { 
	Value val(rapidjson::kObjectType);
	//NS_SERIALISER::ChangeData(&val, my, rapidjson::StringRef(_modelFileName.c_str()));
    Value soundlist(rapidjson::kObjectType);

    //std::string fullpath;
    for (int i = 0; i < _sounds.size(); ++i)
    {
        data& d = _sounds.at(i);
        Value str(rapidjson::kStringType);
        str.SetString(rapidjson::StringRef(d.path));
        //soundlist.PushBack(str, global_alloc);
        NS_SERIALISER::ChangeData(&soundlist, d.name, str);
    }
    NS_SERIALISER::ChangeData(&val, "ListOfSound", soundlist);

	return val;
}
