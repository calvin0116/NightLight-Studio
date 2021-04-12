#include "SystemAudio.h"
#include "../Component/Components.h" // G_ECMANAGER

// Tracy
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"

bool SystemAudio::_isPlaying = false;
bool SystemAudio::_isLoaded = false;
bool SystemAudio::_isInited = false;

void SystemAudio::Init()
{
  // Create FMOD System Once
  FMOD::System_Create(&_system);
  // Init FMOD System
  _system->init(s_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
  // Create Channel Groups
  _system->createChannelGroup("BGM", &_bgm);
  _system->createChannelGroup("SFX", &_sfx);
  _bgm->addGroup(_BGMui);
  _sfx->addGroup(_SFXui);
  // Set Master Channel
  _system->getMasterChannelGroup(&_master);
  // Pool all channels under master channel
  _master->addGroup(_bgm);
  _master->addGroup(_sfx);
  // Set 3D settings
  _system->set3DSettings(1.0f, s_UNITS_PER_METER, 1.0f);
  //_system->set3DRolloffCallback();

  // Load Audio list
  LoadList();
  // Load stuff here
  LoadAudios();

  // Register receiver to message
  r.AttachHandler("TogglePlay", &SystemAudio::HandleTogglePlay, this);
}

void SystemAudio::Update()
{
  if (_isPlaying)
  {
    if (_isInited)
    {
      PlayOnAwake();
    }
    else
    {
      _isInited = true;
      MyGameInit();
    }
  }

  // Update channels to free when playing ends.
  _system->update();

  // Get Listener to update it's position
  auto lisItr = G_ECMANAGER->begin<ComponentListener>();
  auto lisItrEnd = G_ECMANAGER->end<ComponentListener>();
  for (; lisItr != lisItrEnd; ++lisItr)
  {
    ComponentListener* lis = G_ECMANAGER->getComponent<ComponentListener>(lisItr);
    // Err check
    if (lis == nullptr)
    {
      std::string error = "Audio::Update::lis is nullptr";
      TracyMessage(error.c_str(), error.size());
      SPEEDLOG(error);
      continue;
    }
    if (lis->isActive)
    {
      ComponentTransform* pos = G_ECMANAGER->getComponent<ComponentTransform>(lisItr);
      // Err check
      if (pos == nullptr)
      {
        std::string error = "Audio::Update::pos is nullptr";
        TracyMessage(error.c_str(), error.size());
        SPEEDLOG(error);
        continue;
      }
      // Listener position
      FMOD_VECTOR listenerPos;
      listenerPos.x = pos->_position.x;
      listenerPos.y = pos->_position.y;
      listenerPos._z = pos->_position.z;

      // Front vector
      FMOD_VECTOR frontVec;
      NlMath::Vec3 forwardVec = lis->_front;
      NlMath::Matrix4x4 rot;
      NlMath::Mtx44RotYDeg(rot, pos->_rotation.y);
      forwardVec = NlMath::MulT(rot, forwardVec);
      frontVec.x = forwardVec.x;
      frontVec.y = forwardVec.y;
      frontVec._z = forwardVec.z;
      // Up vector
      const FMOD_VECTOR upVec{ 0.0f, 1.0f, 0.0f };
      // Update listener position  
      _system->set3DListenerAttributes(0, &listenerPos, nullptr, &frontVec, &upVec);
    }
  }

  // Get object with currently playing 3D audio to update their position.
  auto audItr = G_ECMANAGER->begin<ComponentLoadAudio>();
  auto audItrEnd = G_ECMANAGER->end<ComponentLoadAudio>();
  for (; audItr != audItrEnd; ++audItr)
  {
    ComponentLoadAudio* aud = G_ECMANAGER->getComponent<ComponentLoadAudio>(audItr);
    // Err check
    if (aud == nullptr)
    {
      std::string error = "Audio::Update::aud is nullptr";
      TracyMessage(error.c_str(), error.size());
      SPEEDLOG(error);
      continue;
    }

    if (aud->isActive)
    {
      LocalVector<ComponentLoadAudio::data>& AudVec = aud->MyAudios;
      const size_t size = AudVec.size();
      for (size_t i = 0; i < size; ++i)
      {
        ComponentLoadAudio::data& MyData = AudVec.at(i);
        // Audio is active, 3D and is playing on a channel
        if (MyData.channel)
        {
          ComponentTransform* trans = G_ECMANAGER->getComponent<ComponentTransform>(audItr);
          // Err check
          if (trans == nullptr)
          {
            std::string error = "Audio::Update::trans is nullptr";
            TracyMessage(error.c_str(), error.size());
            SPEEDLOG(error);
            continue;
          }
          bool playing = false;
          MyData.channel->isPlaying(&playing);
          if (playing && MyData.is3D)
          {
            FMOD_VECTOR pos;
            pos.x = trans->_position.x;
            pos.y = trans->_position.y;
            pos._z = trans->_position.z;
            MyData.channel->set3DAttributes(&pos, nullptr);
            //FMOD_Channel_Set3DAttributes((FMOD_CHANNEL*)chnl, &pos, &vel)
          }
          else if (!playing)
            MyData.channel = nullptr;
        }
      }
    }
  }
}

void SystemAudio::Exit()
{

  //Save Audio List
  SaveList();
  // Release Channels
  _BGMui->release();
  _SFXui->release();
  _bgm->release();
  _sfx->release();
  _master->release();

  // Close and release system
  _system->close();
  _system->release();


  DestroyInstance();
}

void SystemAudio::PlayOnAwake()
{  // Get object with currently playing 3D audio to update their position.
  auto audItr = G_ECMANAGER->begin<ComponentLoadAudio>();
  auto audItrEnd = G_ECMANAGER->end<ComponentLoadAudio>();
  for (; audItr != audItrEnd; ++audItr)
  {
    ComponentLoadAudio* aud = G_ECMANAGER->getComponent<ComponentLoadAudio>(audItr);
    // Err check
    if (aud == nullptr)
    {
      std::string error = "Audio::Update::aud is nullptr";
      TracyMessage(error.c_str(), error.size());
      SPEEDLOG(error);
      continue;
    }

    if (aud->isActive)
    {
      LocalVector<ComponentLoadAudio::data>& AudVec = aud->MyAudios;
      for (ComponentLoadAudio::data& MyData : AudVec)
      {
        if (MyData.playOnAwake)
        {
          // !!!!!!!!!Play Audio here
          Play(aud->objId, MyData);
          // Played, turn onawake off.
          MyData.playOnAwake = false;
        }
      }
    }
  }
}

void SystemAudio::LoadAudios()
{
  // Previously loaded sounds, clear them.
  if (!Sounds.empty())
  {
    // Release sounds here
    for (FMOD::Sound* sound : Sounds)
      sound->release();
    Sounds.clear();
  }

  size_t index = 0;
  for (const std::string& path : Audios)
  {
    // Don't load empty paths
    if (path.empty() || path == "")
      continue;
    FMOD::Sound* tempSound;
    _system->createSound(path.c_str(), FMOD_INIT_NORMAL, 0, &tempSound);
    Sounds.push_back(tempSound);
    ++index;
  }
}

void SystemAudio::Play(int entity, ComponentLoadAudio::data& MyData)
{
  // Err check
  if (entity < 0)
  {
    std::string error = "Audio::Play for entity that doesnt exist!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }
  Entity en = G_ECMANAGER->getEntity(entity);
  ComponentLoadAudio* aud = en.getComponent<ComponentLoadAudio>();
  // Err check
  if (aud == nullptr)
  {
    std::string error = "Audio::Play::aud is nullptr";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  //LocalVector<ComponentLoadAudio::data>& audio = aud->MyAudios;
  //ComponentLoadAudio::data& MyData = audio.at(_index);
  const int& aud_index = MyData.index;
  if (aud_index < 0 || aud_index >= Audios.size())
  {
    std::string error = "Audio " + std::to_string(aud_index) + " is out of range";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }
  FMOD::Sound*& sound = Sounds[aud_index];
  // Stop previously playing audio if any.
  MyData.channel->stop();
  // Play sound but pause it first to set attributes.
  if (MyData.isBGM)
  {
    if (MyData.isUI)
      _system->playSound(sound, _BGMui, true, &MyData.channel);
    else
      _system->playSound(sound, _bgm, true, &MyData.channel);
  }
  else
  {
    if (MyData.isUI)
      _system->playSound(sound, _SFXui, true, &MyData.channel);
    else
      _system->playSound(sound, _sfx, true, &MyData.channel);
  }
  if (MyData.isLoop)
    MyData.channel->setMode(FMOD_LOOP_NORMAL);
  else
    MyData.channel->setMode(FMOD_LOOP_OFF);
  if (MyData.is3D)
  {
    MyData.channel->setMode(FMOD_3D);
    MyData.channel->setMode(FMOD_3D_LINEARROLLOFF);
    MyData.channel->set3DMinMaxDistance(MyData.minDist * s_UNITS_PER_METER, MyData.maxDist * s_UNITS_PER_METER);
    // Get position to set initial pos
    ComponentTransform* trans = en.getComponent<ComponentTransform>();
    // Err check
    if (trans == nullptr)
    {
      std::string error = "Audio::Play::trans is nullptr, id:" + entity;
      TracyMessage(error.c_str(), error.size());
      SPEEDLOG(error);
      return;
    }
    FMOD_VECTOR pos;
    pos.x = trans->_position.x;
    pos.y = trans->_position.y;
    pos._z = trans->_position.z;
    MyData.channel->set3DAttributes(&pos, nullptr);
  }
  else
    MyData.channel->setMode(FMOD_2D);

  // Set volume
  MyData.channel->setVolume(MyData.volume);

  // Unpause sound to start playing it
  MyData.channel->setPaused(false);
}

void SystemAudio::Stop(int entity, ComponentLoadAudio::data& MyData)
{
  // Err check
  if (entity < 0)
  {
    std::string error = "Audio::Stop for entity that doesnt exist!";
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }
  Entity en = G_ECMANAGER->getEntity(entity);
  ComponentLoadAudio* aud = en.getComponent<ComponentLoadAudio>();
  // Err check
  if (aud == nullptr)
  {
    std::string error = "Audio::Stop::aud is nullptr, id:" + entity;
    TracyMessage(error.c_str(), error.size());
    SPEEDLOG(error);
    return;
  }

  // Stop audio
  MyData.channel->stop();
}

void SystemAudio::MyGameExit()
{
  // Get aud comp to stop all from playing
  auto audItr = G_ECMANAGER->begin<ComponentLoadAudio>();
  auto audItrEnd = G_ECMANAGER->end<ComponentLoadAudio>();
  for (; audItr != audItrEnd; ++audItr)
  {
    ComponentLoadAudio* aud = G_ECMANAGER->getComponent<ComponentLoadAudio>(audItr);
    for (ComponentLoadAudio::data& MyData : aud->MyAudios)
      MyData.channel->stop();
  }
}

void SystemAudio::SaveList()
{
  AudioListParser.CleanDoc();

  Value* aud_str_list = new Value;
  aud_str_list->SetArray();
  //Value aud_str_list(rapidjson::kArrayType);
  for (int i = 0; i < Audios.size(); ++i)
    aud_str_list->PushBack(rapidjson::StringRef(Audios[i].c_str()), global_alloc);
  //NS_SERIALISER::ChangeData(aud_str_list, "index", rapidjson::StringRef(Audios[i].c_str()) );


  AudioListParser.AddData("Audio List", aud_str_list, &global_alloc);
  AudioListParser.Save();

  //delete aud_str_list;
}

void SystemAudio::LoadList()
{
  AudioListParser.Load();
  if (AudioListParser.GetDoc().MemberCount() == 0)
    return;

  for (Value::ConstMemberIterator itr = AudioListParser.GetDoc().MemberBegin(); itr != AudioListParser.GetDoc().MemberEnd(); ++itr)
  {
    if (itr->name == "Audio List")
    {
      auto aud_list = AudioListParser["Audio List"].GetArray();
      for (unsigned i = 0; i < aud_list.Size(); ++i)
      {
        Audios.push_back(aud_list[i].GetString());
      }
    }
  }
}

void SystemAudio::GameExit()
{
  //_isPlaying = false;
  _isInited = false;
  MyGameExit();
}

void SystemAudio::HandleTogglePlay(MessageTogglePlay& msg)
{
  // Handle msg here. Only Before Play MSG
  if (msg.GetID() != "TogglePlay")
    return;
  _isPlaying = msg.isPlaying;
  if (!_isPlaying && _isInited)
  {
    _isInited = false;
    MyGameExit();
  }
}