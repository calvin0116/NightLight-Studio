#include "SystemAudio.h"

const float SystemAudio::s_UNITS_PER_METER = 100.0f;

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

void SystemAudio::PlayOnce(const std::string& _name)
{
  MyAudioMap::iterator it = _sounds.find(_name);
  if (it != _sounds.end())
  {
    FMOD::Channel* temp;
    _system->playSound(it->second, _sfx, true, &temp);
    temp->setMode(FMOD_LOOP_OFF);
    temp->setMode(FMOD_2D);
    temp->setPaused(false);

    //// Setting group channel
    //ErrorCheck(it->second.second->setChannelGroup(SFX));
  }
}

int SystemAudio::Play3DLoop(const std::string& _name, const NlMath::Vector3D& _pos)
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
        //m_Channels[retVal]->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 100.0f * m_fUnitsPerMeter);
        FMOD_VECTOR fmodPos = { _pos.x, _pos.y, _pos.z };
        FMOD_VECTOR fmodVel = { 100.0f, 0.0f, 0.0f };
        _channels[retVal]->set3DAttributes(&fmodPos, &fmodVel);
        _channels[retVal]->setPaused(false);

        //// Setting group channel
        //ErrorCheck(it->second.second->setChannelGroup(BGM));
        break;
      }
    }
  }
  return retVal;
}

int SystemAudio::Play3DOnce(const std::string& name, const NlMath::Vector3D& _pos)
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
        //ErrorCheck(temp->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 5.0f * m_fUnitsPerMeter));
        FMOD_VECTOR fmodPos = { _pos.x, _pos.y, _pos.z };
        _channels[retVal]->set3DAttributes(&fmodPos, nullptr);
        _channels[retVal]->setPaused(false);
        //// Setting group channel
        //ErrorCheck(it->second.second->setChannelGroup(SFX));
        break;
      }
    }
  }
  return retVal;
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
}

void SystemAudio::Init()
{

}

void SystemAudio::Update()
{
  // position update here
  _system->update();
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