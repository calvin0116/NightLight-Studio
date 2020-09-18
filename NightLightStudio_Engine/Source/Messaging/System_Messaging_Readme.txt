SYSTEM MESSAGING README HOW 2 USE

Classes to Note:
IMessage
SystemReceiver
SystemBroadcaster

/*******************************************************************************************/

[ IMessage ]
Interface Class for messages.
All Messages MUST Inherit this class.
Whatever data the message has is up to you.

/*******************************************************************************************/

[ SystemReceiver ]
All System Receivers have an ID.
Objects using it can either inherit or hold a System_Receiver
Needs to be accessible outside of object in some way.
- Functions
void AttachHandler(const std::string& messageID, void(*handler)(T&) )
void AttachHandler(const std::string& messageId, void(T::* handler)(U&), Y* obj)

messageID - If listener receives a message with this ID, invoke handler function
handler - a void ptr that has a reference to a IMessage or child class.
ie. void testFunc(IMessage&)
    class TestMessage : public IMessage {};
    void testFunc2(TestMessage&)

Note: Registered Handlers have unique MessageIDs. If you want a MessageID to run 2 different
functions, use another Receiver. (Generally 1 Reciever per class maximum, so 1 MessageID should
only really correspond to 1 function use)

RemoveHandler( const std::string& messageID )
Removes a Handler via ID.

GetHandler( const std::string& messageID )
Gets a Handler via ID. - Not really used.


/*******************************************************************************************/  
  
[ SystemBroadcaster ]
All System Broadcasters have an ID.

- Functions
Register (const std::string msg_ID, System_Receiver &receiver)
Registers an ID to a Listener.
Everytime a message with the ID is processed, it will be sent to the Receiver.

Unregister (const std::string msg_ID, System_Receiver &receiver)
Unregisters an ID to a Receiver

ProcessMessage( IMessage& msg )
Sends the message out to the various Receivers


/*******************************************************************************************/

TLDR;

HOW 2 USE

using namespace SystemMessaging;

struct TestMessage : public IMessage 
{
	TestMessage(std::string str) : IMessage(str) {}
	std::string str;
};
void tFunc(TestMessage& tmsg)
{
	std::cout << tmsg.str << std::endl;
}

int main()
{
	// Init Observables and Listeners
	SystemBroadcast brd{ "brd" };
	SystemReceiver rec{ "rec" };

	// Attaching ID to check
	rec.AttachHandler("ID", tFunc);
	brd.Register("ID", rec);

	// Sending message
	TestMessage t{ "ID" };
	t.str = "TEST MESSAGE";

	// Sends message to all listeners that use messages with its ID
	brd.ProcessMessage(t);

	return 0;
}


NOTE:
There is an in-built GLOBAL_SYSTEM_BROADCAST in SystemBroadcast.h
This will automatically register ALL Handlers that are attached to Receivers
If you have attached a Handler to a Receiver, as long as you send out the appropriate message in
the Global Broadcast, it will reach the Receiver (Assuming there are no conflicts between MessageIDs)