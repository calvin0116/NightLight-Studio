#include "SystemAudio.h"

#include "../Component/Components.h" // G_ECMANAGER
//#include "../Component/ComponentAudio.h" // Audio Component Data

#include "../Input/SystemInput.h" // For testing

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
         glm::vec3 _pos = G_ECMANAGER->getEntity(_id).getComponent<TransformComponent>()->_position;
        //m_Channels[retVal]->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 100.0f * m_fUnitsPerMeter);
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

void SystemAudio::Play3DOnce(const std::string& name, const int _id)
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
        glm::vec3 _pos = G_ECMANAGER->getEntity(_id).getComponent<TransformComponent>()->_position;
        _fmodPos = { _pos.x, _pos.y, _pos.z };
        _channels[retVal]->set3DAttributes(&_fmodPos, nullptr);
        _channels[retVal]->setPaused(false);
        _objIDs[retVal] = _id;
        //ErrorCheck(temp->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 5.0f * m_fUnitsPerMeter));
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
        _fmodPos = { x, y, z };
        _channels[retVal]->set3DAttributes(&_fmodPos, nullptr);
        _channels[retVal]->setPaused(false);
        //ErrorCheck(temp->set3DMinMaxDistance(0.5f * m_fUnitsPerMeter, 5.0f * m_fUnitsPerMeter));
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
  // For testing
  //LoadSound("Asset/Sounds/TestAudio.ogg", "TestAudio");

  // Numpad 0 = Normal both speakers
  SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TestAudio", SystemInput_ns::IKEY_NUMPAD_0, "AudioTest", SystemInput_ns::OnRelease, [this]()
    {
      if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IKEY_NUMPAD_0))
      {
        PlayOnce("TestAudio");
      }
    });
  // Numpad 1 = 3D BGM on left side
  SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Test1Audio", SystemInput_ns::IKEY_NUMPAD_1, "Audio1Test", SystemInput_ns::OnRelease, [this]()
    {
      if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IKEY_NUMPAD_1))
      {
        Play3DOnce("TestAudio", -1.0f, 0.0f, 0.0f);
      }
    });
  // Numpad 2 = 3D BGM on right side
  SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Test2Audio", SystemInput_ns::IKEY_NUMPAD_2, "Audio2Test", SystemInput_ns::OnRelease, [this]()
    {
      if (SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IKEY_NUMPAD_2))
      {
        Play3DOnce("TestAudio", 1.0f, 0.0f, 0.0f);
      }
    });
}

void SystemAudio::GameLoad()
{
}

void SystemAudio::GameInit()
{
  // Loading
  auto itr = G_ECMANAGER->begin<ComponentLoadAudio>();
  auto itrEnd = G_ECMANAGER->end<ComponentLoadAudio>();
  for (; itr != itrEnd; ++itr)
  {
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
}

void SystemAudio::Update()
{
  // position update here
  _system->update();
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