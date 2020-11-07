#pragma once
#include "..\SystemIMessage.h"

struct MessageScriptRequest : public SystemMessaging::IMessage
{
  MessageScriptRequest(int uniqueTagID) : IMessage("ScriptRequest"), tagID(uniqueTagID), _pScript(nullptr) {}
  // Must be unique
  int tagID;
  IScript* _pScript;
};