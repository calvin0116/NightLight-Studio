#include "SystemAudio.h"
#include "../Component/Components.h" // G_ECMANAGER

// Tracy
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"

//const float SystemAudio::s_UNITS_PER_METER = 100.0f;
//const int listenerTag = 200;
//const float minDist = 0.5f;
//const float maxDist = 5.0f;
//bool SystemAudio::_isPlaying = false;
//
//void SystemAudio::LoadSound(const std::string& _soundPath, const std::string& _name)
//{
//  auto it = _sounds.find(_name);
//  if (it == _sounds.end())
//  {
//    FMOD::Sound* temp;
//    _system->createSound(_soundPath.c_str(), FMOD_LOOP_OFF, 0, &temp);
//    _sounds.emplace(_name, temp);
//  }
//}
//
//void SystemAudio::ReleaseSounds()
//{
//  for (auto& [name, sound] : _sounds)
//    sound->release();
//  _sounds.clear();
//}
//
//void SystemAudio::Pause(const int _channelID)
//{
//  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
//    return;
//  _channels[_channelID]->setPaused(true);
//}
//
//void SystemAudio::Unpause(const int _channelID)
//{
//  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
//    return;
//  _channels[_channelID]->setPaused(false);
//}
//
//void SystemAudio::Stop(const int _channelID)
//{
//  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
//    return;
//  _channels[_channelID]->stop();
//}
//
//void SystemAudio::SetVol(const int _channelID, float _vol)
//{
//  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
//    return;
//  _channels[_channelID]->setVolume(_vol);
//}
//
//int SystemAudio::PlayBGM(const std::string& _name)
//{
//  MyAudioMap::iterator it = _sounds.find(_name);
//  int retVal = -1;
//  if (it != _sounds.end())
//  {
//    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
//    {
//      bool isPlaying;
//      _channels[retVal]->isPlaying(&isPlaying);
//      if (!isPlaying)
//      {
//        _system->playSound(it->second, _bgm, true, _channels + retVal);
//        _channels[retVal]->setMode(FMOD_LOOP_NORMAL);
//        _channels[retVal]->setMode(FMOD_2D);
//        _channels[retVal]->setPaused(false);
//        break;
//      }
//    }
//  }
//  return retVal;
//}
//
//int SystemAudio::PlayOnce(const std::string& _name)
//{
//  MyAudioMap::iterator it = _sounds.find(_name);
//  int retVal = -1;
//  if (it != _sounds.end())
//  {
//    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
//    {
//      bool isPlaying;
//      _channels[retVal]->isPlaying(&isPlaying);
//      if (!isPlaying)
//      {
//        FMOD::Channel* temp;
//        _system->playSound(it->second, _sfx, true, &temp);
//        temp->setMode(FMOD_LOOP_OFF);
//        temp->setMode(FMOD_2D);
//        temp->setPaused(false);
//        break;
//      }
//    }
//  }
//  return retVal;
//}
//
//// Name of sound to loop and entity ID's position to follow
//int SystemAudio::Play3DLoop(const std::string& _name, const int _id)
//{
//  MyAudioMap::iterator it = _sounds.find(_name);
//  int retVal = -1;
//  if (it != _sounds.end())
//  {
//    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
//    {
//      bool isPlaying;
//      _channels[retVal]->isPlaying(&isPlaying);
//      if (!isPlaying)
//      {
//        _system->playSound(it->second, _bgm, true, _channels + retVal);
//        _channels[retVal]->setMode(FMOD_LOOP_NORMAL);
//        _channels[retVal]->setMode(FMOD_3D);
//        _channels[retVal]->setMode(FMOD_3D_LINEARROLLOFF);
//         glm::vec3 _pos = G_ECMANAGER->getEntity(_id).getComponent<TransformComponent>()->_position;
//        //m_Channels[retVal]->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 100.0f * m_fUnitsPerMeter);
//         _channels[retVal]->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
//        _fmodPos = { _pos.x, _pos.y, _pos.z };
//        //FMOD_VECTOR fmodVel = { 100.0f, 0.0f, 0.0f };
//        _channels[retVal]->set3DAttributes(&_fmodPos, nullptr);
//        _channels[retVal]->setPaused(false);
//        _objIDs[retVal] = _id;
//
//        //// Setting group channel
//        //ErrorCheck(it->second.second->setChannelGroup(BGM));
//        break;
//      }
//    }
//  }
//  return retVal;
//}
//
//int SystemAudio::Play3DOnce(const std::string& name, const int _id)
//{
//  MyAudioMap::iterator it = _sounds.find(name);
//  int retVal = -1;
//  if (it != _sounds.end())
//  {
//    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
//    {
//      bool isPlaying;
//      _channels[retVal]->isPlaying(&isPlaying);
//      if (!isPlaying)
//      {
//        FMOD::Channel* temp;
//        _system->playSound(it->second, _sfx, true, &temp);
//        temp->setMode(FMOD_LOOP_OFF);
//        temp->setMode(FMOD_3D);
//        temp->setMode(FMOD_3D_LINEARROLLOFF);
//        temp->setMode(FMOD_3D);
//        glm::vec3 _pos = G_ECMANAGER->getEntity(_id).getComponent<TransformComponent>()->_position;
//        _fmodPos = { _pos.x, _pos.y, _pos.z };
//        temp->set3DAttributes(&_fmodPos, nullptr);
//        temp->setPaused(false);
//        //_objIDs[retVal] = _id;
//        _channels[retVal]->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
//        break;
//      }
//    }
//  }
//  return retVal;
//}
//
////// Overloaded function for testing only! do not use this!
////void SystemAudio::Play3DOnce(const std::string& name, float x, float y, float z)
////{
////  MyAudioMap::iterator it = _sounds.find(name);
////  if (it != _sounds.end())
////  {
////    for (int retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
////    {
////      bool isPlaying;
////      _channels[retVal]->isPlaying(&isPlaying);
////      if (!isPlaying)
////      {
////        _system->playSound(it->second, _sfx, true, _channels + retVal);
////        _channels[retVal]->setMode(FMOD_LOOP_OFF);
////        _channels[retVal]->setMode(FMOD_3D);
////        _channels[retVal]->setMode(FMOD_3D_LINEARROLLOFF);
////        _fmodPos = { x, y, z };
////        _channels[retVal]->set3DAttributes(&_fmodPos, nullptr);
////        _channels[retVal]->setPaused(false);
////        //ErrorCheck(temp->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 5.0f * m_fUnitsPerMeter));
////        _channels[retVal]->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
////        //FMOD_VECTOR fmodPos = { _pos.x, _pos.y, _pos.z };
////    /*    _channels[retVal]->set3DAttributes(&fmodPos, nullptr);
////        _channels[retVal]->setPaused(false);*/
////        //// Setting group channel
////        //ErrorCheck(it->second.second->setChannelGroup(SFX));
////        break;
////      }
////    }
////  }
////}
//
//void SystemAudio::FadeOut(const int _channelID, const float _sec)
//{
//  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
//    return;
//  _channelIDs.emplace(_channelID, _sec);
//  //_channels[_channelID]->setVolume(_vol);
//}
//
//void SystemAudio::Load()
//{
//  // Create FMOD System Once
//  FMOD::System_Create(&_system);
//  // Init FMOD System
//  _system->init(s_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
//  // Create Channel Groups
//  _system->createChannelGroup("BGM", &_bgm);
//  _system->createChannelGroup("SFX", &_sfx);
//  // Set Master Channel
//  _system->getMasterChannelGroup(&_master);
//  // Pool all channels under master channel
//  _master->addGroup(_bgm);
//  _master->addGroup(_sfx);
//  // Set 3D settings
//  _system->set3DSettings(1.0f, s_UNITS_PER_METER, 1.0f);
//  //_system->set3DRolloffCallback();
//}
//
//void SystemAudio::Init()
//{
//  // Register receiver to message
//  r.AttachHandler("TogglePlay", &SystemAudio::HandleTogglePlay, this);
//}
//
//void SystemAudio::GameLoad()
//{
//
//}
//
//void SystemAudio::GameInit()
//{
//
//}
//
//void SystemAudio::GameExit()
//{
//  for (int i = 0; i < s_MAX_CHANNELS; ++i)
//  {
//    _channels[i]->stop();
//  }
//}
//
//void SystemAudio::MyGameInit()
//{
//  // For testing
//  //LoadSound("asset/Sounds/TestAudio.ogg", "TestAudio");
//  // Loading
//  ReleaseSounds();
//  auto itr = G_ECMANAGER->begin<ComponentLoadAudio>();
//  auto itrEnd = G_ECMANAGER->end<ComponentLoadAudio>();
//  for (; itr != itrEnd; ++itr)
//  {
//
//    std::string outStr = "SystemAudio::My Game Init: "; outStr.append(std::to_string(G_ECMANAGER->getObjId(itr)));
//    //std::cout << G_ECMANAGER->getObjId(itr) << std::endl;
//    TracyMessageL("SystemAudio::My Game Init: System Audio Game Init Obj ID:");
//    TracyMessage(outStr.c_str(), outStr.size());
//
//    SPEEDLOG("SystemAudio::My Game Init: System Audio Game Init Obj ID:");
//    SPEEDLOG(outStr);
//
//    // Load the following audios from load audio component
//    ComponentLoadAudio* myComp = G_ECMANAGER->getComponent<ComponentLoadAudio>(itr);
//    //for (const auto& [path, name] : myComp->_sounds)
//    //  LoadSound(path, name);
//  }
//  // Init
//  auto itr1 = G_ECMANAGER->begin<AudioComponent>();
//  auto itrEnd1 = G_ECMANAGER->end<AudioComponent>();
//  for (; itr1 != itrEnd1; ++itr1)
//  {
//    AudioComponent* myComp = G_ECMANAGER->getComponent<AudioComponent>(itr1);
//    // Component not active, skip it
//    if (!myComp->_isActive)
//      continue;
//    // It is active, loop through the sounds available
//    for (AudioComponent::Data& data : myComp->_sounds)
//    {
//      if (data._isActive && data._playOnStart)
//      {
//        if (data._loop)
//        {
//          if (data._is3D)
//            Play3DLoop(data._name, G_ECMANAGER->getObjId(itr1));
//          else
//            PlayBGM(data._name);
//        }
//        else
//        {
//          if (data._is3D)
//            Play3DOnce(data._name, G_ECMANAGER->getObjId(itr1));
//          else
//            PlayOnce(data._name);
//        }
//      }
//    }
//  }
//
//  // get listener
//  auto itr2 = G_ECMANAGER->begin<ComponentTransform>();
//  auto itrEnd2 = G_ECMANAGER->end<ComponentTransform>();
//  for (; itr2 != itrEnd2; ++itr2)
//  {
//    ComponentTransform* trans = G_ECMANAGER->getComponent<ComponentTransform>(itr2);
//    if (trans->_tag == listenerTag)
//    {
//      //std::cout << "Found! " << std::endl;
//      _listenerVecPos = &trans->_position;
//    }
//  }
//}
//
//void SystemAudio::Update()
//{
//  // Tracy
//  // Zone Color: Green
//  ZoneScopedNC("Audio", 0x1df52f);
//
//  // position update here
//  _system->update();
//  if (_listenerVecPos)
//  {
//    _listenerPos.x = _listenerVecPos->x;
//    _listenerPos.y = _listenerVecPos->y;
//    _listenerPos._z = _listenerVecPos->z;
//    //std::cout
//    //  << " x:" << _listenerPos.x
//    //  << " y:" << _listenerPos.y
//    //  << " z:" << _listenerPos._z << std::endl;
//  }
//  /** IMPORTANT UNFINISHED CODE **/
//  // Set player's forward and up vector here WHEN AVAILABLE
//  _system->set3DListenerAttributes(0, &_listenerPos, nullptr, nullptr, nullptr);
//  // Update 3D positions
//  for (int i = 0; i < s_MAX_CHANNELS; ++i)
//  {
//    bool isPlaying;
//    FMOD_MODE fMode;
//    _channels[i]->isPlaying(&isPlaying);
//    _channels[i]->getMode(&fMode);
//    if (!isPlaying || fMode != FMOD_3D)
//      continue;
//    glm::vec3 _pos = G_ECMANAGER->getEntity(_objIDs[i]).getComponent<TransformComponent>()->_position;
//    _fmodPos = { _pos.x, _pos.y, _pos.z };
//    // Set position
//    _channels[i]->set3DAttributes(&_fmodPos, nullptr);
//  }
//
//  // Fade out audios
//  if (!_channelIDs.empty())
//  {
//    MyChnnlMap::iterator begin = _channelIDs.begin();
//    MyChnnlMap::iterator end = _channelIDs.end();
//    while (begin != end)
//    {
//      float vol;
//      _channels[begin->first]->getVolume(&vol);
//      vol -= DELTA_T->real_dt / begin->second; // (1.0f * fixedDt) / duration - decreased vol per second.
//      _channels[begin->first]->setVolume(vol);
//      if (vol <= 0.0f)
//      {
//        _channels[begin->first]->stop();
//        begin = _channelIDs.erase(begin);
//      }
//      else
//        ++begin;
//    }
//  }
//}
//
//void SystemAudio::Free()
//{
//  // Release Channels
//  _bgm->release();
//  _sfx->release();
//
//  // Release DSPs
//  // Not implemented yet...
//}
//
//void SystemAudio::Exit()
//{
//  // Close and release system
//  _system->close();
//  _system->release();
//
//  DestroyInstance();
//}

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
  // Set Master Channel
  _system->getMasterChannelGroup(&_master);
  // Pool all channels under master channel
  _master->addGroup(_bgm);
  _master->addGroup(_sfx);
  // Set 3D settings
  _system->set3DSettings(1.0f, s_UNITS_PER_METER, 1.0f);
  //_system->set3DRolloffCallback();

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
      NlMath::Vec3 forwardVec{0.0f, 0.0f, 1.0f};
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
        if (MyData.isActive && MyData.channel)
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
  // Release Channels
  _bgm->release();
  _sfx->release();

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
      const size_t size = AudVec.size();
      for (size_t i = 0; i < size; ++i)
      {
        ComponentLoadAudio::data& MyData = AudVec.at(i);
        if (MyData.isActive && MyData.playOnAwake)
        {
          // !!!!!!!!!Play Audio here
          Play(aud->objId);
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
    FMOD::Sound* tempSound;
    _system->createSound(path.c_str(), FMOD_INIT_NORMAL, 0, &tempSound);
    Sounds.push_back(tempSound);
    ++index;
  }
}

void SystemAudio::Play(int entity)
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

  LocalVector<ComponentLoadAudio::data>& audio = aud->MyAudios;
  const size_t size = audio.size();
  for (int i = 0; i < size; ++i)
  {
    ComponentLoadAudio::data& MyData = audio.at(i);
    // Set isActive to true to play it;
    MyData.isActive = true;
    FMOD::Sound*& sound = Sounds[MyData.index];
    // Play sound but pause it first to set attributes.
    if (MyData.isBGM)
      _system->playSound(sound, _bgm, true, &MyData.channel);
    else
      _system->playSound(sound, _sfx, true, &MyData.channel);
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
        std::string error = "Audio::Play::trans is nullptr";
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
    // Unpause sound to start playing it
    MyData.channel->setPaused(false);
  }
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