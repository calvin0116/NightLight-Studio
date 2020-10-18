#ifndef _SYSTEM_IMESSAGE
#define _SYSTEM_IMESSAGE

#include <string>

namespace SystemMessaging
{
	class IMessage
	{
	protected:
		std::string _id;
	public:
		IMessage() : _id{ "\n" } {}
		IMessage(std::string id) : _id{ id } {}

		std::string GetID() { return _id; }

		virtual ~IMessage() = default;
	};
}

#endif
