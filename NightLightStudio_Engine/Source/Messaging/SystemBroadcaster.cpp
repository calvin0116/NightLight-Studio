#include "SystemBroadcaster.h"
#include <algorithm>

// Global Broacaster
SystemMessaging::GlobalSystemBroadcaster GLOBAL_SYSTEM_BROADCAST;

namespace SystemMessaging
{
	SystemBroadcast::SystemBroadcast() : _id{ "\n" }
	{
		GLOBAL_SYSTEM_BROADCAST.Register(_id, this);
	}

	SystemBroadcast::SystemBroadcast(std::string id) : _id{ id }
	{
		if (_id != GLOBAL_BROADCAST_ID)
			GLOBAL_SYSTEM_BROADCAST.Register(_id, this);
	}

	void SystemBroadcast::Register(const std::string msg_ID, SystemReceiver* receiver)
	{
		std::pair<std::multimap<std::string, SystemReceiver*>::iterator,
			std::multimap<std::string, SystemReceiver*>::iterator> range
			= _receiverMap.equal_range(msg_ID);
		bool reg = true;
		for (auto it = range.first; it != range.second; ++it)
		{
			if (it->second == receiver)
			{
				reg = false;
				break;
			}
		}
		if (reg)
		{
			_receiverMap.insert(ReceiverMap::value_type(msg_ID, receiver));
			receiver->_allRegBroadcasters[this]++;
		}
	}

	void SystemBroadcast::Unregister(const std::string msg_ID, SystemReceiver* receiver)
	{
		std::pair<std::multimap<std::string, SystemReceiver*>::iterator,
			std::multimap<std::string, SystemReceiver*>::iterator> range
			= _receiverMap.equal_range(msg_ID);
		for (auto it = range.first; it != range.second; ++it)
		{
			if (it->second == receiver)
			{
				_receiverMap.erase(it);
				break;
			}
		}
		if (receiver->_allRegBroadcasters.find(this) != std::end(receiver->_allRegBroadcasters))
			receiver->_allRegBroadcasters[this]--;
	}

	void SystemBroadcast::ProcessMessage(IMessage& msg)
	{
		std::pair<std::multimap<std::string, SystemReceiver*>::iterator,
			std::multimap<std::string, SystemReceiver*>::iterator> range
			= _receiverMap.equal_range(msg.GetID());
		for (auto it = range.first; it != range.second; ++it)
		{
			// Runs only if function is valid
			MESSAGE_HANDLER temp = it->second->GetHandler(msg.GetID());
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
			i->second->_allRegBroadcasters.erase(this);
		}
		GLOBAL_SYSTEM_BROADCAST.Unregister(_id);
	}


	GlobalSystemBroadcaster::GlobalSystemBroadcaster()
		: SystemBroadcast{ GLOBAL_BROADCAST_ID }
	{
	}

	GlobalSystemBroadcaster::~GlobalSystemBroadcaster()
	{
	}

	void GlobalSystemBroadcaster::Register(const std::string& b_ID, const std::string& msg_ID, SystemReceiver* receiver)
	{
		decltype(_broadcastMap)::iterator iter = _broadcastMap.find(b_ID);
		if (iter != std::end(_broadcastMap))
		{
			iter->second->Register(msg_ID, receiver);
		}
	}

	void GlobalSystemBroadcaster::Register(const std::string& b_ID, SystemBroadcast* broadcaster)
	{
		_broadcastMap.emplace(decltype(_broadcastMap)::value_type(b_ID, broadcaster));
	}

	void GlobalSystemBroadcaster::Register(const std::string msg_ID, SystemReceiver* receiver)
	{
		SystemBroadcast::Register(msg_ID, receiver);
	}

	void GlobalSystemBroadcaster::Unregister(const std::string& b_ID, const std::string& msg_ID, SystemReceiver* receiver)
	{
		decltype(_broadcastMap)::iterator iter = _broadcastMap.find(b_ID);
		if (iter != std::end(_broadcastMap))
		{
			iter->second->Unregister(msg_ID, receiver);
		}
	}

	void GlobalSystemBroadcaster::Unregister(const std::string& b_ID)
	{
		if (b_ID != GLOBAL_BROADCAST_ID)
			_broadcastMap.erase(b_ID);
	}

	void GlobalSystemBroadcaster::Unregister(const std::string& msg_ID, SystemReceiver* receiver)
	{
		SystemBroadcast::Register(msg_ID, receiver);
	}

	void GlobalSystemBroadcaster::ProcessMessage(const std::string& b_ID, IMessage& msg)
	{
		decltype(_broadcastMap)::iterator iter = _broadcastMap.find(b_ID);
		if (iter != std::end(_broadcastMap))
		{
			iter->second->ProcessMessage(msg);
		}
	}

	void GlobalSystemBroadcaster::ProcessMessage(IMessage& msg)
	{
		SystemBroadcast::ProcessMessage(msg);
	}
}
