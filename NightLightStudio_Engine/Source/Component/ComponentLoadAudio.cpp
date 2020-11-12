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
    auto sound_array = val["ListOfSound"].GetArray();
    for (int i = 0; i < sound_array.Size(); ++i)
    {
      fs::path cur_path_name = sound_array[i].GetString();
      data MyData;
      std::string MyName = cur_path_name.stem().string();
      std::string MyPath = cur_path_name.parent_path().string() + "/" + MyName + cur_path_name.extension().string();
      strcpy_s(MyData.name, 128, MyName.c_str());
      strcpy_s(MyData.path, 512, MyPath.c_str());
      _sounds.push_back(MyData);
    }
  }
}

inline Value ComponentLoadAudio::Write() { 
	Value val(rapidjson::kObjectType);
	//NS_SERIALISER::ChangeData(&val, my, rapidjson::StringRef(_modelFileName.c_str()));
	return val;
}
