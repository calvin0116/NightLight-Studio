#include "SystemBroadcaster.h"
#include <algorithm>

// Global Broacaster
NS_MESSAGING::SystemBroadcast G_SYSTEM_BROADCAST ("GLOBAL_BROADCAST");

namespace NS_MESSAGING
{
	SystemBroadcast::SystemBroadcast() : _id{ "\n" }
	{
	}

	SystemBroadcast::SystemBroadcast(std::string id) : _id{ id }
	{
	}

	void SystemBroadcast::Register(const std::string msg_ID, SystemReceiver& receiver)
	{
		_receiverMap.insert(ReceiverMap::value_type(msg_ID, receiver));
		receiver._allRegBroadcasters[this]++;
	}

	void SystemBroadcast::Unregister(const std::string msg_ID, SystemReceiver& receiver)
	{
		std::pair<std::multimap<std::string, SystemReceiver&>::iterator,
			std::multimap<std::string, SystemReceiver&>::iterator> range
			= _receiverMap.equal_range(msg_ID);
		for (auto it = range.first; it != range.second; ++it)
		{
			if (&it->second == &receiver)
			{
				_receiverMap.erase(it);
				break;
			}
		}
		if (receiver._allRegBroadcasters[this])
			receiver._allRegBroadcasters[this]--;
	}

	void SystemBroadcast::ProcessMessage(IMessage& msg)
	{
		std::pair<std::multimap<std::string, SystemReceiver&>::iterator,
			std::multimap<std::string, SystemReceiver&>::iterator> range
			= _receiverMap.equal_range(msg.GetID());
		for (auto it = range.first; it != range.second; ++it)
		{
			// Runs only if function is valid
			MESSAGE_HANDLER temp = it->second.GetHandler(msg.GetID());
			if (temp)
				temp(msg);
		}
	}

	std::string SystemBroadcast::GetID()
	{
		return _id;
	}

	SystemBroadcast::~SystemBroadcast()
	{
		// If Broadcast is destroyed before Receiver, removes itself from Receivers
		for (ReceiverMap::iterator i = std::begin(_receiverMap); i != std::end(_receiverMap); ++i)
		{
			i->second._allRegBroadcasters.erase(this);
		}
	}
}
