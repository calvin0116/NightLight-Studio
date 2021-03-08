#include "SystemAudio.h"
#include "../Component/Components.h" // G_ECMANAGER

// Tracy
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"

const float SystemAudio::s_UNITS_PER_METER = 100.0f;
const int listenerTag = 200;
const float minDist = 0.5f;
const float maxDist = 5.0f;
bool SystemAudio::_isPlaying = false;

void SystemAudio::LoadSound(const std::string& _soundPath, const std::string& _name)
{
  auto it = _sounds.find(_name);
  if (it == _sounds.end())
  {
    FMOD::Sound* temp;
    _system->createSound(_soundPath.c_str(), FMOD_LOOP_OFF, 0, &temp);
    _sounds.emplace(_name, temp);
  }
}

void SystemAudio::ReleaseSounds()
{
  for (auto& [name, sound] : _sounds)
    sound->release();
  _sounds.clear();
}

void SystemAudio::Pause(const int _channelID)
{
  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
    return;
  _channels[_channelID]->setPaused(true);
}

void SystemAudio::Unpause(const int _channelID)
{
  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
    return;
  _channels[_channelID]->setPaused(false);
}

void SystemAudio::Stop(const int _channelID)
{
  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
    return;
  _channels[_channelID]->stop();
}

void SystemAudio::SetVol(const int _channelID, float _vol)
{
  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
    return;
  _channels[_channelID]->setVolume(_vol);
}

int SystemAudio::PlayBGM(const std::string& _name)
{
  MyAudioMap::iterator it = _sounds.find(_name);
  int retVal = -1;
  if (it != _sounds.end())
  {
    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
    {
      bool isPlaying;
      _channels[retVal]->isPlaying(&isPlaying);
      if (!isPlaying)
      {
        _system->playSound(it->second, _bgm, true, _channels + retVal);
        _channels[retVal]->setMode(FMOD_LOOP_NORMAL);
        _channels[retVal]->setMode(FMOD_2D);
        _channels[retVal]->setPaused(false);
        break;
      }
    }
  }
  return retVal;
}

int SystemAudio::PlayOnce(const std::string& _name)
{
  MyAudioMap::iterator it = _sounds.find(_name);
  int retVal = -1;
  if (it != _sounds.end())
  {
    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
    {
      bool isPlaying;
      _channels[retVal]->isPlaying(&isPlaying);
      if (!isPlaying)
      {
        FMOD::Channel* temp;
        _system->playSound(it->second, _sfx, true, &temp);
        temp->setMode(FMOD_LOOP_OFF);
        temp->setMode(FMOD_2D);
        temp->setPaused(false);
        break;
      }
    }
  }
  return retVal;
}

// Name of sound to loop and entity ID's position to follow
int SystemAudio::Play3DLoop(const std::string& _name, const int _id)
{
  MyAudioMap::iterator it = _sounds.find(_name);
  int retVal = -1;
  if (it != _sounds.end())
  {
    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
    {
      bool isPlaying;
      _channels[retVal]->isPlaying(&isPlaying);
      if (!isPlaying)
      {
        _system->playSound(it->second, _bgm, true, _channels + retVal);
        _channels[retVal]->setMode(FMOD_LOOP_NORMAL);
        _channels[retVal]->setMode(FMOD_3D);
        _channels[retVal]->setMode(FMOD_3D_LINEARROLLOFF);
         glm::vec3 _pos = G_ECMANAGER->getEntity(_id).getComponent<TransformComponent>()->_position;
        //m_Channels[retVal]->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 100.0f * m_fUnitsPerMeter);
         _channels[retVal]->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
        _fmodPos = { _pos.x, _pos.y, _pos.z };
        //FMOD_VECTOR fmodVel = { 100.0f, 0.0f, 0.0f };
        _channels[retVal]->set3DAttributes(&_fmodPos, nullptr);
        _channels[retVal]->setPaused(false);
        _objIDs[retVal] = _id;

        //// Setting group channel
        //ErrorCheck(it->second.second->setChannelGroup(BGM));
        break;
      }
    }
  }
  return retVal;
}

int SystemAudio::Play3DOnce(const std::string& name, const int _id)
{
  MyAudioMap::iterator it = _sounds.find(name);
  int retVal = -1;
  if (it != _sounds.end())
  {
    for (retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
    {
      bool isPlaying;
      _channels[retVal]->isPlaying(&isPlaying);
      if (!isPlaying)
      {
        _system->playSound(it->second, _sfx, true, _channels + retVal);
        _channels[retVal]->setMode(FMOD_LOOP_OFF);
        _channels[retVal]->setMode(FMOD_3D);
        _channels[retVal]->setMode(FMOD_3D_LINEARROLLOFF);
        glm::vec3 _pos = G_ECMANAGER->getEntity(_id).getComponent<TransformComponent>()->_position;
        _fmodPos = { _pos.x, _pos.y, _pos.z };
        _channels[retVal]->set3DAttributes(&_fmodPos, nullptr);
        _channels[retVal]->setPaused(false);
        _objIDs[retVal] = _id;
        //ErrorCheck(temp->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 5.0f * m_fUnitsPerMeter));
        _channels[retVal]->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
        //FMOD_VECTOR fmodPos = { _pos.x, _pos.y, _pos.z };
    /*    _channels[retVal]->set3DAttributes(&fmodPos, nullptr);
        _channels[retVal]->setPaused(false);*/
        //// Setting group channel
        //ErrorCheck(it->second.second->setChannelGroup(SFX));
        break;
      }
    }
  }
  return retVal;
}

// Overloaded function for testing only! do not use this!
void SystemAudio::Play3DOnce(const std::string& name, float x, float y, float z)
{
  MyAudioMap::iterator it = _sounds.find(name);
  if (it != _sounds.end())
  {
    for (int retVal = 0; retVal < s_MAX_CHANNELS; ++retVal)
    {
      bool isPlaying;
      _channels[retVal]->isPlaying(&isPlaying);
      if (!isPlaying)
      {
        _system->playSound(it->second, _sfx, true, _channels + retVal);
        _channels[retVal]->setMode(FMOD_LOOP_OFF);
        _channels[retVal]->setMode(FMOD_3D);
        _channels[retVal]->setMode(FMOD_3D_LINEARROLLOFF);
        _fmodPos = { x, y, z };
        _channels[retVal]->set3DAttributes(&_fmodPos, nullptr);
        _channels[retVal]->setPaused(false);
        //ErrorCheck(temp->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 5.0f * m_fUnitsPerMeter));
        _channels[retVal]->set3DMinMaxDistance(minDist * s_UNITS_PER_METER, maxDist * s_UNITS_PER_METER);
        //FMOD_VECTOR fmodPos = { _pos.x, _pos.y, _pos.z };
    /*    _channels[retVal]->set3DAttributes(&fmodPos, nullptr);
        _channels[retVal]->setPaused(false);*/
        //// Setting group channel
        //ErrorCheck(it->second.second->setChannelGroup(SFX));
        break;
      }
    }
  }
}

void SystemAudio::FadeOut(const int _channelID, const float _sec)
{
  if (_channelID < 0 || _channelID >= s_MAX_CHANNELS)
    return;
  _channelIDs.emplace(_channelID, _sec);
  //_channels[_channelID]->setVolume(_vol);
}

void SystemAudio::Load()
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
}

void SystemAudio::Init()
{
  // Register receiver to message
  r.AttachHandler("TogglePlay", &SystemAudio::HandleTogglePlay, this);
}

void SystemAudio::GameLoad()
{
}

void SystemAudio::GameInit()
{

}

void SystemAudio::GameExit()
{
  for (int i = 0; i < s_MAX_CHANNELS; ++i)
  {
    _channels[i]->stop();
  }
}

void SystemAudio::MyGameInit()
{
  // For testing
  //LoadSound("asset/Sounds/TestAudio.ogg", "TestAudio");
  // Loading
  auto itr = G_ECMANAGER->begin<ComponentLoadAudio>();
  auto itrEnd = G_ECMANAGER->end<ComponentLoadAudio>();
  for (; itr != itrEnd; ++itr)
  {
    //std::cout << G_ECMANAGER->getObjId(itr) << std::endl;
    TracyMessageL("SystemAudio::My Game Init: System Audio Game Init Obj ID:");
    TracyMessageL(std::string("SystemAudio::My Game Init: ").append(std::to_string(G_ECMANAGER->getObjId(itr))).c_str());

    SPEEDLOG("SystemAudio::My Game Init: System Audio Game Init Obj ID:");
    SPEEDLOG(std::string("SystemAudio::My Game Init: ").append(std::to_string(G_ECMANAGER->getObjId(itr))));

    // Load the following audios from load audio component
    ComponentLoadAudio* myComp = G_ECMANAGER->getComponent<ComponentLoadAudio>(itr);
    for (const auto& [path, name] : myComp->_sounds)
      LoadSound(path, name);
  }
  // Init
  auto itr1 = G_ECMANAGER->begin<AudioComponent>();
  auto itrEnd1 = G_ECMANAGER->end<AudioComponent>();
  for (; itr1 != itrEnd1; ++itr1)
  {
    AudioComponent* myComp = G_ECMANAGER->getComponent<AudioComponent>(itr1);
    // Component not active, skip it
    if (!myComp->_isActive)
      continue;
    // It is active, loop through the sounds available
    for (AudioComponent::Data& data : myComp->_sounds)
    {
      if (data._isActive && data._playOnStart)
      {
        if (data._loop)
        {
          if (data._is3D)
            Play3DLoop(data._name, G_ECMANAGER->getObjId(itr1));
          else
            PlayBGM(data._name);
        }
        else
        {
          if (data._is3D)
            Play3DOnce(data._name, G_ECMANAGER->getObjId(itr1));
          else
            PlayOnce(data._name);
        }
      }
    }
  }

  // get listener
  auto itr2 = G_ECMANAGER->begin<ComponentTransform>();
  auto itrEnd2 = G_ECMANAGER->end<ComponentTransform>();
  for (; itr2 != itrEnd2; ++itr2)
  {
    ComponentTransform* trans = G_ECMANAGER->getComponent<ComponentTransform>(itr2);
    if (trans->_tag == listenerTag)
    {
      //std::cout << "Found! " << std::endl;
      _listenerVecPos = &trans->_position;
    }
  }
}

void SystemAudio::Update()
{
  // Tracy
  // Zone Color: Green
  ZoneScopedNC("Audio", 0x1df52f);

  // position update here
  _system->update();
  if (_listenerVecPos)
  {
    _listenerPos.x = _listenerVecPos->x;
    _listenerPos.y = _listenerVecPos->y;
    _listenerPos._z = _listenerVecPos->z;
    //std::cout
    //  << " x:" << _listenerPos.x
    //  << " y:" << _listenerPos.y
    //  << " z:" << _listenerPos._z << std::endl;
  }
  /** IMPORTANT UNFINISHED CODE **/
  // Set player's forward and up vector here WHEN AVAILABLE
  _system->set3DListenerAttributes(0, &_listenerPos, nullptr, nullptr, nullptr);
  // Update 3D positions
  for (int i = 0; i < s_MAX_CHANNELS; ++i)
  {
    bool isPlaying;
    FMOD_MODE fMode;
    _channels[i]->isPlaying(&isPlaying);
    _channels[i]->getMode(&fMode);
    if (!isPlaying || fMode != FMOD_3D)
      continue;
    glm::vec3 _pos = G_ECMANAGER->getEntity(_objIDs[i]).getComponent<TransformComponent>()->_position;
    _fmodPos = { _pos.x, _pos.y, _pos.z };
    // Set position
    _channels[i]->set3DAttributes(&_fmodPos, nullptr);
  }

  // Fade out audios
  if (!_channelIDs.empty())
  {
    MyChnnlMap::iterator begin = _channelIDs.begin();
    MyChnnlMap::iterator end = _channelIDs.end();
    while (begin != end)
    {
      float vol;
      _channels[begin->first]->getVolume(&vol);
      vol -= DELTA_T->real_dt / begin->second; // (1.0f * fixedDt) / duration - decreased vol per second.
      _channels[begin->first]->setVolume(vol);
      if (vol <= 0.0f)
      {
        _channels[begin->first]->stop();
        begin = _channelIDs.erase(begin);
      }
      else
        ++begin;
    }
  }
}

void SystemAudio::Free()
{
  // Release Channels
  _bgm->release();
  _sfx->release();

  // Release DSPs
  // Not implemented yet...
}

void SystemAudio::Exit()
{
  // Close and release system
  _system->close();
  _system->release();

  DestroyInstance();
}

void SystemAudio::HandleTogglePlay(MessageTogglePlay& msg)
{
  // Handle msg here. Only Before Play MSG
  if (msg.GetID() != "TogglePlay")
    return;
  _isPlaying = msg.isPlaying;
  if (_isPlaying && _Inited == false)
  {
    _Inited = true;
    MyGameInit();
  }
  else if (!_isPlaying)
  {
    ReleaseSounds();
    _Inited = false;
  }
}