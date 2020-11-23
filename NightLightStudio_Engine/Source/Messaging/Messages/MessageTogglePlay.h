#pragma once
#include "..\SystemIMessage.h"

struct MessageTogglePlay : public SystemMessaging::IMessage
{
  MessageTogglePlay(bool _playing, std::string str) : IMessage(str), isPlaying(_playing) {}
  
  // Must be unique
  bool isPlaying;
};