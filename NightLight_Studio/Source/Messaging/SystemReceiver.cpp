/******************************************************************************/
/*!
\file		SystemListener.cpp
\project	Aris
\author 	Primary: Leong Jun Ming
\par    	email: l.junming\@digipen.edu
\date   	December 03, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "SystemReceiver.h"
#include "SystemBroadcaster.h"
#include <algorithm>    // std::for_each

namespace SystemMessaging
{
	void SystemReceiver::GlobalRegister(const std::string& msgId)
	{
		GLOBAL_SYSTEM_BROADCAST.Register(msgId, *this);
	}
	void SystemReceiver::GlobalUnregister(const std::string& msgId)
	{
		GLOBAL_SYSTEM_BROADCAST.Unregister(msgId, *this);
	}

	SystemReceiver::SystemReceiver() : _id { "\n" }
	{
	}
	SystemReceiver::SystemReceiver(std::string id) : _id { id }
	{
	}

	void SystemReceiver::RemoveHandler(const std::string& messageID)
	{
		_handleMap.erase(messageID);
	}

	MESSAGE_HANDLER SystemReceiver::GetHandler(const std::string& messageId)
	{
		decltype(_handleMap)::iterator temp = _handleMap.find(messageId);
		if (temp != std::end(_handleMap))
			return (temp->second);
		else
			return nullptr;
	}

	void SystemReceiver::SendToAllBroadcast(IMessage& msg)
	{
		for (std::pair<SystemBroadcast*, unsigned> i : _allRegBroadcasters)
		{
			if (i.second)
				i.first->ProcessMessage(msg);
		}
	}

	void SystemReceiver::SendToSpecificBroadcast(std::string brdID, IMessage& msg)
	{
		for (std::pair<SystemBroadcast*, unsigned> i : _allRegBroadcasters)
		{
			if (i.second)
			{
				if (i.first->GetID()._Equal(brdID))
					i.first->ProcessMessage(msg);
			}
		}
	}

	std::string SystemReceiver::GetID()
	{
		return _id;
	}

	// Removes itself automatically from all Broadcasters
	SystemReceiver::~SystemReceiver()
	{
		std::for_each(std::begin(_allRegBroadcasters), std::end(_allRegBroadcasters), 
			[=](std::pair<SystemBroadcast*, unsigned> obs) 
			{
				for (decltype(_handleMap)::iterator i = std::begin(_handleMap); i != std::end(_handleMap); ++i)
				{
					obs.first->Unregister(i->first, *this);
				}
			});
	}
}
