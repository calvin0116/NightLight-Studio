#pragma once
#include "..\SystemIMessage.h"

struct MessageTogglePlay : public SystemMessaging::IMessage
{
  MessageTogglePlay(bool _playing) : IMessage("TogglePlay"), isPlaying(_playing) {}
  // Must be unique
  bool isPlaying;
};