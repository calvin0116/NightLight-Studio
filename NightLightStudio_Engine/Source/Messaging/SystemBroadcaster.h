/******************************************************************************/
/*!
\file		SystemObservable.h
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

#ifndef _SYSTEM_BROADCASTER
#define _SYSTEM_BROADCASTER

#include "SystemReceiver.h"

namespace SystemMessaging
{
	class SystemBroadcast
	{
		typedef std::multimap<std::string, SystemReceiver&> ReceiverMap;

		std::string _id;
		ReceiverMap _receiverMap;
	public:
		SystemBroadcast();
		SystemBroadcast(std::string id);

		// Register a Message ID with a receiver - The receiver will now receive messages of this ID
		// Duplicates of the same MessageID and Reciever MAY exist - This will cause those Receivers to run multiple times
		void Register(const std::string msg_ID, SystemReceiver& receiver);
		// Unregister a Message ID with a receiver - The receiver will no longer receive messages of this ID
		void Unregister(const std::string msg_ID, SystemReceiver& receiver);

		// Broadcasts Message to all Registered Receivers
		void ProcessMessage(IMessage& msg);

		// Returns ID to this Broadcaster
		std::string GetID();
		
		~SystemBroadcast();
	};
}

extern SystemMessaging::SystemBroadcast GLOBAL_SYSTEM_BROADCAST;

#endif