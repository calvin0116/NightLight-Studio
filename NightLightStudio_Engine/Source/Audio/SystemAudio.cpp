#include "SystemAudio.h"

const float SystemAudio::s_UNITS_PER_METER = 100.0f;

void SystemAudio::OnFirstStart()
{  // Create FMOD System Once
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

void SystemAudio::Load(const std::string& _soundPath, const std::string& _name)
{
  //// Create placeholder first
  //auto emplaceVal = m_Sounds.emplace(_UserDefinedName, );
  //// Create sound and channel
  //MyAudioMap::iterator it = emplaceVal.first;
  //if (emplaceVal.second)
  //  ErrorCheck(system->createSound(_soundPath.c_str(), FMOD_LOOP_OFF, 0, &(it->second.first)), "create " + _UserDefinedName);
  //else
  //  std::cout << "Unable to load " + _UserDefinedName + ". Duplicate Name." << std::endl;
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
      _system->playSound(it->second, _bgm, false, 
    }
  }
}

void SystemAudio::Load()
{

}

void SystemAudio::Init()
{

}

void SystemAudio::Update(float dt)
{
}

void SystemAudio::Exit()
{
}

void SystemAudio::Free()
{
}

void SystemAudio::Unload()
{
}
