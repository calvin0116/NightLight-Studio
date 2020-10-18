#ifndef _SYSTEM_RECEIVER
#define _SYSTEM_RECEIVER

#include "../../framework.h"

#include "SystemIMessage.h"
#include <map>
#include <functional>
#include <set>

namespace SystemMessaging
{
	typedef std::function<void(IMessage&)> MESSAGE_HANDLER;
	//typedef void(*MESSAGE_HANDLER)(IMessage&);

	class SystemBroadcast;

	namespace
	{
		template <typename T>
		class GetFuncArgVal;
		template <class T>
		class GetFuncArgVal<std::function<void(T)>>
		{
		public:
			typedef T arg;
		};

		// ACTUAL FUNCTION THAT IS STORED IN std::function
		template <typename FUNC, typename BASE>
		void FuncParamCast(FUNC f, BASE& b)
		{
			std::invoke(f, static_cast<typename GetFuncArgVal<FUNC>::arg>(b));
		}

		// FOR USE WITH STATIC FUNCTIONS
		template <typename T>
		MESSAGE_HANDLER Message_BindToFunc(void(*t)(T&))
		{
			return std::bind(FuncParamCast<decltype(std::function<void(T&)>(t)), IMessage>, t, std::placeholders::_1);
		}

		// FOR USE WITH MEMBER FUNCTIONS
		template <typename T, typename U, typename Y>
		MESSAGE_HANDLER BindMemFuncOneParam(void(T::* memFunc)(U&), Y* myObj)
		{
			std::function<void(U&)> newMemFn = std::bind(memFunc, myObj, std::placeholders::_1);
			return std::bind(FuncParamCast<decltype(newMemFn), IMessage>, newMemFn, std::placeholders::_1);
			//return std::bind(memFunc, myObj, std::placeholders::_1);
		}
	}
	class SystemReceiver
	{
		friend SystemBroadcast;
		std::string _id;
		std::map<std::string, MESSAGE_HANDLER> _handleMap;
		std::map<SystemBroadcast*, unsigned int> _allRegBroadcasters;

		void GlobalRegister(const std::string& msgId);
		void GlobalUnregister(const std::string& msgId);

	public:
		SystemReceiver();
		SystemReceiver(std::string id);

		// SFINAE, Ensures function MUST HAVE a parameter that uses IMessage as a Base Type
		// Default Functions
		
		// Attach a handle (Static Function) to the Receiver
		template <typename T,
			class = typename std::enable_if<std::is_base_of<IMessage, T>::value>::type>
		void AttachHandler(const std::string& messageId, void(*handler)(T&))
		{
			_handleMap.emplace(std::pair<std::string, MESSAGE_HANDLER>(messageId, Message_BindToFunc(handler)));
			GlobalRegister(messageId);
		}
		// Attach a handle (Member Function) to the Receiver
		template <typename T, typename U, typename Y,
			class = typename std::enable_if<std::is_base_of<IMessage, U>::value>::type>
			void AttachHandler(const std::string& messageId, void(T::* handler)(U&), Y* obj)
		{
			_handleMap.emplace(std::pair<std::string, MESSAGE_HANDLER>(messageId, BindMemFuncOneParam(handler, obj)));
			GlobalRegister(messageId);
		}
		// Attach a handle (Function) to the Receiver
		void AttachHandler(const std::string& messageId, MESSAGE_HANDLER handler)
		{
			_handleMap.emplace(std::pair<std::string, MESSAGE_HANDLER>(messageId, handler));
			GlobalRegister(messageId);
		}

		// Removes a handle (Function) to the Receiver
		void RemoveHandler(const std::string& messageID);
		// Gets the Function associated with a handle
		MESSAGE_HANDLER GetHandler(const std::string& messageId);
		
		// Sends a message to all Broadcasts that have this Receiver Registered
		void SendToAllBroadcast(IMessage& msg);
		// Sends a message to a Specific Broadcast
		void SendToSpecificBroadcast(std::string brdID, IMessage& msg);

		// Returns ID to this Receiver
		std::string GetID();

		~SystemReceiver();
	};
}

#endif