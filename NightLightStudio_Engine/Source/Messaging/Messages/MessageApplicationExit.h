#pragma once
#include "..\SystemIMessage.h"

struct MessageApplicationExit : public SystemMessaging::IMessage
{
  MessageApplicationExit() : IMessage("ApplicationExit") {}
};