// SpeedLog
#include "../Log/SpeedLog.h"
#include "ComponentLoadAudio.h"
#include "../tracy-master/Tracy.hpp"
//#include "Components.h"   // inherit required functions
// Audio System
#include "../Audio/SystemAudio.h" // TO play sound

void ComponentLoadAudio::AudioData::Play()
{
  SYS_AUDIO->Play(objID, *this);
}

void ComponentLoadAudio::AudioData::SetLoop(bool _loop)
{
  isLoop = _loop;
  if (_loop)
    channel->setMode(FMOD_LOOP_NORMAL);
  else
    channel->setMode(FMOD_LOOP_OFF);
}

void ComponentLoadAudio::AudioData::SetVolume(float _vol)
{
  volume = _vol;
  channel->setVolume(_vol);
}

void ComponentLoadAudio::AudioData::Set3D(bool _3d)
{
  is3D = _3d;
  if (_3d)
  {
    channel->setMode(FMOD_3D);
    channel->setMode(FMOD_3D_LINEARROLLOFF);
    channel->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
  }
  else
    channel->setMode(FMOD_2D);
}

void ComponentLoadAudio::AudioData::SetMinDist(float _val)
{
  minDist = _val;
  channel->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
}

void ComponentLoadAudio::AudioData::SetMaxDist(float _val)
{
  maxDist = _val;
  channel->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
}

bool ComponentLoadAudio::AudioData::GetMute() const
{
  bool ret = true;
  channel->getMute(&ret);
  return ret;
}

void ComponentLoadAudio::AudioData::SetMute(bool _mute)
{
  channel->setMute(_mute);
}

bool ComponentLoadAudio::AudioData::GetPause() const
{
  bool ret = true;
  channel->getPaused(&ret);
  return ret;
}

void ComponentLoadAudio::AudioData::SetPause(bool _pause)
{
  channel->setPaused(_pause);
}

ComponentLoadAudio::ComponentLoadAudio()
{
  strcpy_s(ser_name, "AudioManager");
}

ComponentLoadAudio::~ComponentLoadAudio()
{

}

void	ComponentLoadAudio::Read(Value& val)
{
  MyAudios.clear();
  for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
  {
    auto data_list = itr->value.GetArray();
    for (unsigned i = 0; i < data_list.Size(); ++i)
    {
        auto aud_val = data_list[i].GetObjectA();
        
        data aud_obj;   //create dummy object
        aud_obj.objID = objId;

        for (Value::ConstMemberIterator itr2 = aud_val.MemberBegin(); itr2 != aud_val.MemberEnd(); ++itr2)
        {
            if (itr2->name == "index")
            {
                aud_obj.index = itr2->value.GetInt();        // index of sound in audio imgui
            }
            else if (itr2->name == "isBGM")
            {
                aud_obj.isBGM = itr2->value.GetBool();       // Whether sound is a BGM/Ambience or SFX
            }
            else if (itr2->name == "isLoop")
            {
                aud_obj.isLoop = itr2->value.GetBool();      // whether this sound is looping
            }
            else if (itr2->name == "playOnAwake")
            {
                aud_obj.playOnAwake = itr2->value.GetBool(); // whether to play on isActive = true
            }
            else if (itr2->name == "volume")
            {
                aud_obj.volume = itr2->value.GetFloat();     // Volume of sound 0.0f - 1.0f
            }
            else if (itr2->name == "is3D")
            {
                aud_obj.is3D = itr2->value.GetBool();        // Whether sound is 3D
            }
            else if (itr2->name == "minDist")
            {
                // 3D variables
                aud_obj.minDist = itr2->value.GetFloat();
            }
            else if (itr2->name == "maxDist")
            {
                aud_obj.maxDist = itr2->value.GetFloat();
            }
        }
        MyAudios.push_back(aud_obj);
    }

  }
}

inline Value ComponentLoadAudio::Write()
{
  Value val(rapidjson::kObjectType);
  //NS_SERIALISER::ChangeData(&val, my, rapidjson::StringRef(_modelFileName.c_str()));

  //for (size_t i = 0; i < MySize; ++i)
  //{
  //  data& d = MyAudios[i];

  //  //NS_SERIALISER::ChangeData(&soundData, std::to_string(i), )
  //}
  Value aud_data_list(rapidjson::kArrayType);
  ////std::string fullpath;
  for (int i = 0; i < MyAudios.size(); ++i)
  {
      data& d = MyAudios.at(i);
      Value aud_data(rapidjson::kObjectType);

      NS_SERIALISER::ChangeData(&aud_data, "index", d.index);
      NS_SERIALISER::ChangeData(&aud_data, "isBGM", d.isBGM);
      NS_SERIALISER::ChangeData(&aud_data, "isLoop", d.isLoop);
      NS_SERIALISER::ChangeData(&aud_data, "playOnAwake", d.playOnAwake);
      NS_SERIALISER::ChangeData(&aud_data, "volume", d.volume);
      NS_SERIALISER::ChangeData(&aud_data, "is3D", d.is3D);
      NS_SERIALISER::ChangeData(&aud_data, "minDist", d.minDist);
      NS_SERIALISER::ChangeData(&aud_data, "maxDist", d.maxDist);

      aud_data_list.PushBack(aud_data, global_alloc);
  }
  NS_SERIALISER::ChangeData(&val, "ListOfSound", aud_data_list);

  return val;
}
