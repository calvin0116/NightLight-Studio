// SpeedLog
#include "../Log/SpeedLog.h"
#include "ComponentLoadAudio.h"
#include "../tracy-master/Tracy.hpp"
//#include "Components.h"   // inherit required functions
// Audio System
#include "../Audio/SystemAudio.h" // TO play sound

ComponentLoadAudio::ComponentLoadAudio()
{
  strcpy_s(ser_name, "AudioManager");
}

ComponentLoadAudio::~ComponentLoadAudio()
{

}

bool ComponentLoadAudio::GetPlay(int _index)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::GetPlay _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return false;
  }

  bool isPlaying = false;
  MyAudios.at(_index).channel->isPlaying(&isPlaying);
  return isPlaying;
}

void ComponentLoadAudio::Play(int _index)
{
  SYS_AUDIO->Play(objId, _index);
}

bool ComponentLoadAudio::GetMute(int _index)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::GetMute _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return true;
  }

  bool isMute = false;
  MyAudios.at(_index).channel->getMute(&isMute);
  return isMute;
}

void ComponentLoadAudio::Mute(int _index, bool _mute)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::Mute _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  //SYS_AUDIO->Mute(objId, _index, _mute);
  MyAudios.at(_index).channel->setMute(_mute);
}

bool ComponentLoadAudio::GetPause(int _index)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::GetPause _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return true;
  }

  bool isPause = false;
  MyAudios.at(_index).channel->getPaused(&isPause);
  return isPause;
}

void ComponentLoadAudio::Pause(int _index, bool _pause)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::Pause _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  //SYS_AUDIO->Mute(objId, _index, _mute);
  MyAudios.at(_index).channel->setPaused(_pause);
}

void ComponentLoadAudio::SetLoop(int _index, bool _loop)
{  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::SetLoop _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  data& MyData = MyAudios.at(_index);
  MyData.isLoop = _loop;
  if (_loop)
    MyData.channel->setMode(FMOD_LOOP_NORMAL);
  else
    MyData.channel->setMode(FMOD_LOOP_OFF);
}

void ComponentLoadAudio::SetVolume(int _index, float _vol)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::SetVolume _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  //SYS_AUDIO->Mute(objId, _index, _mute);
  data& MyData = MyAudios.at(_index);
  MyData.volume = _vol;
  MyData.channel->setVolume(_vol);
}

void ComponentLoadAudio::SetMinDist(int _index, float _dist)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::SetMinDist _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  //SYS_AUDIO->Mute(objId, _index, _mute);
  data& MyData = MyAudios.at(_index);
  MyData.minDist = _dist;
  MyData.channel->set3DMinMaxDistance(_dist, MyData.maxDist);
}

void ComponentLoadAudio::SetMaxDist(int _index, float _dist)
{
  // Err check
  if (_index < 0 || _index >= MyAudios.size())
  {
    std::string error = "AudioComponent::SetMaxDist _index out of range!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  //SYS_AUDIO->Mute(objId, _index, _mute);
  data& MyData = MyAudios.at(_index);
  MyData.maxDist = _dist;
  MyData.channel->set3DMinMaxDistance(MyData.minDist, _dist);
}

void	ComponentLoadAudio::Read(Value& val)
{
  for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
  {
    auto data_list = itr->value.GetArray();
    for (unsigned i = 0; i < data_list.Size(); ++i)
    {

    }

  }
}

inline Value ComponentLoadAudio::Write()
{
  Value val(rapidjson::kObjectType);
  //NS_SERIALISER::ChangeData(&val, my, rapidjson::StringRef(_modelFileName.c_str()));
  Value soundData(rapidjson::kObjectType);

  //for (size_t i = 0; i < MySize; ++i)
  //{
  //  data& d = MyAudios[i];

  //  //NS_SERIALISER::ChangeData(&soundData, std::to_string(i), )
  //}

  ////std::string fullpath;
  //for (int i = 0; i < _sounds.size(); ++i)
  //{
  //    data& d = _sounds.at(i);
  //    Value str(rapidjson::kStringType);
  //    str.SetString(rapidjson::StringRef(d.path));
  //    //soundlist.PushBack(str, global_alloc);
  //    NS_SERIALISER::ChangeData(&soundlist, d.name, str);
  //}
  //NS_SERIALISER::ChangeData(&val, "ListOfSound", soundlist);

  return val;
}
