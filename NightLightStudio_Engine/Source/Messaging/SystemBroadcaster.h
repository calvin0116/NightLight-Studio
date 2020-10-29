#ifndef _SYSTEM_BROADCASTER
#define _SYSTEM_BROADCASTER

#include "SystemReceiver.h"

namespace SystemMessaging
{
	class SystemBroadcast
	{
		typedef std::multimap<std::string, SystemReceiver*> ReceiverMap;

		std::string _id;
		ReceiverMap _receiverMap;
	public:
		SystemBroadcast();
		SystemBroadcast(std::string id);

		// Register a Message ID with a receiver - The receiver will now receive messages of this ID
		// Duplicates of the same MessageID and Reciever MAY exist - This will cause those Receivers to run multiple times
		void Register(const std::string msg_ID, SystemReceiver* receiver);
		// Unregister a Message ID with a receiver - The receiver will no longer receive messages of this ID
		void Unregister(const std::string msg_ID, SystemReceiver* receiver);

		// Broadcasts Message to all Registered Receivers
		void ProcessMessage(IMessage& msg);

		// Returns ID to this Broadcaster
		std::string GetID();

		virtual ~SystemBroadcast();
	};

	class GlobalSystemBroadcaster : public SystemBroadcast
	{
		typedef std::map<std::string, SystemBroadcast*> BroadcasterMap;

		BroadcasterMap _broadcastMap;
	public:
		GlobalSystemBroadcaster();
		~GlobalSystemBroadcaster();

		void Register(const std::string& b_ID, const std::string& msg_ID, SystemReceiver* receiver);
		void Register(const std::string& b_ID, SystemBroadcast* broadcaster);
		void Register(const std::string msg_ID, SystemReceiver* receiver);

		void Unregister(const std::string& b_ID, const std::string& msg_ID, SystemReceiver* receiver);
		void Unregister(const std::string& b_ID);
		void Unregister(const std::string& msg_ID, SystemReceiver* receiver);

		void ProcessMessage(const std::string& b_ID, IMessage& msg);
		void ProcessMessage(IMessage& msg);
	};
}

const std::string GLOBAL_BROADCAST_ID = "GLOBAL_BROADCASTER";
extern SystemMessaging::GlobalSystemBroadcaster GLOBAL_SYSTEM_BROADCAST;

#endif