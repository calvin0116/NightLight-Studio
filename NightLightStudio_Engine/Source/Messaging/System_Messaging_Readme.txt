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

CODE EXAMPLE

// THIS IS THE CLASS THAT HOLDS THE MESSAGE
class MessageTest : public SystemMessaging::IMessage
{
public:
	std::string text;
	MessageTest(std::string id, std::string t) : IMessage(id), text{ t } {}
};

// THIS IS A STATIC FUNCTION THAT WILL RUN ON RECEIVING THE MESSAGE
void TestFunc(MessageTest& a)
{
	std::cout << a.text << std::endl;
}

// THIS IS THE CLASS (MEMBER FUNCTION) THAT WILL RUN ON RECEIVING THE MESSAGE
class TestClass
{
public:
	void TestFuncClass(MessageTest& a)
	{
		std::cout << "CLASS " << a.text << std::endl;
	}
};

int main()
{
	// Static function messages
	MessageTest msgTest1("test", "1");
	MessageTest msgTest2("test", "2");
	MessageTest msgTest3("test", "3");
	MessageTest msgTest4("test", "4");
	MessageTest msgTest5("test", "5");

	// Member function messages
	MessageTest msgTest6("test2", "6");
	MessageTest msgTest7("test2", "7");
	MessageTest msgTest8("test2", "8");
	MessageTest msgTest9("test2", "9");
	MessageTest msgTest10("test2", "10");

	// Lambda Messages
	MessageTest msgTest11("test3", "11");
	MessageTest msgTest12("test3", "12");
	MessageTest msgTest13("test3", "13");
	MessageTest msgTest14("test3", "14");
	MessageTest msgTest15("test3", "15");

	// Class to call member function from
	TestClass testClass;

	// Creating a Receiver
	SystemMessaging::SystemReceiver r("ID");

	// Attaching a function to a handler
	// test, test2 and test3 are the Message_IDs that will trigger the respective functions
	// Static Function
	r.AttachHandler("test", TestFunc);
	// Member Function
	r.AttachHandler("test2", &TestClass::TestFuncClass, &testClass);
	// Lambda
	r.AttachHandler<MessageTest>("test3", [](MessageTest& a) { std::cout << "LAMBDA " << a.text << std::endl; });

	// Creating a Broadcaster
	SystemMessaging::SystemBroadcast b("Broadcast");

	// Registering a Message_ID and its Receiver to a Broadcaster
	b.Register("test", &r);
	//b.Register("test2", &r);
	b.Register("test3", &r);

	// Registering a Message_ID and its Receiver to a Broadcaster by its ID
	// In this case we will register via to the Global System Broadcaster
	//GLOBAL_SYSTEM_BROADCAST.Register(b.GetID(), "test", &r);
	GLOBAL_SYSTEM_BROADCAST.Register(b.GetID(), "test2", &r);
	//GLOBAL_SYSTEM_BROADCAST.Register(b.GetID(), "test3", &r);

	// Processes all the messages, in the order of:
	// Global Send All messages
	// Direct send to Broadcaster via Global
	// Send to all Broadcaster registered on Receiver
	// Send to Broadcaster via Receiver
	// Send to Broadcaster Directly
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(msgTest1);
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(b.GetID(), msgTest2);
	r.SendToAllBroadcast(msgTest3);
	r.SendToSpecificBroadcast(GLOBAL_BROADCAST_ID, msgTest4);
	b.ProcessMessage(msgTest5);

	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(msgTest6);
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(b.GetID(), msgTest7);
	r.SendToAllBroadcast(msgTest8);
	r.SendToSpecificBroadcast(GLOBAL_BROADCAST_ID, msgTest9);
	b.ProcessMessage(msgTest10);

	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(msgTest11);
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(b.GetID(), msgTest12);
	r.SendToAllBroadcast(msgTest13);
	r.SendToSpecificBroadcast(GLOBAL_BROADCAST_ID, msgTest14);
	b.ProcessMessage(msgTest15);
}


NOTE:
There is an in-built GLOBAL_SYSTEM_BROADCAST in SystemBroadcast.h
This will automatically register ALL Handlers that are attached to Receivers
This will also automatically register ALL Broadcasters
If you have attached a Handler to a Receiver, as long as you send out the appropriate message in
the Global Broadcast, it will reach the Receiver (Assuming there are no conflicts between MessageIDs)