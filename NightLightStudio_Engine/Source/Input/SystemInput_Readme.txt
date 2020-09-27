SYSTEM INPUT README HOW 2 USE

Classes to Note:
SI_KeyPress
SI_MousePos
SI_Controller

namespace SystemInput_ns

========= TL;DR SCROLL TO THE ENDv=========

/*******************************************************************************************/
                                         SI_KEYPRESS
/*******************************************************************************************/
[ Virtual Key ]

SystemInput_ns::IKEY_...

This is used to get the key you want to check.
This also includes Mouse LButton and RButton.

==============================================

[ Trigger ]

SystemInput_ns::On...

This is used to get the Trigger state. OnHold, OnPress, OnRelease etc.

==============================================

[ SystemKeyPress ]

FUNCTIONS:
bool Update(float)  // Self Explanatory

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Checks only if keys are registered for use
bool GetKeyPress(unsigned int keycode);
bool GetKeyHold(unsigned int keycode);
bool GetKeyRelease(unsigned int keycode);

Exactly as advertised. Note that these only work when an Event 
(Event Does not necessarily need to have a function)
Is registered for use.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Checks if a key is toggled
bool GetKeyToggle(unsigned int keycode);

Checks if a key is in a toggled state, such as CAPSLOCK or NUMLOCK etc.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Checks directly from hardware state
bool GetKeyStateHold(unsigned int keycode);

Gets if a key is being held down directly from Hardware.
Used if you aren't using Events?

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// This is used to create a new event to run
void CreateNewEvent(const std::string& name, unsigned int keycode, std::string identifier, Trigger val, ...)

Adds a new Event to run. NOTE that an Event can have MULTIPLE functions. ("FIRE ACTION event may run multiple different functions at once.")
name        -> Name of the Event
keycode     -> Keycode that triggers the Event
identifier  -> Identifier of the function to run.
val         -> Trigger to run the Event. (Press, Release etc.)

... MAY REFER TO EITHER STATIC OR MEMBER FUNCTIONS.
BOTH REQUIRE void() functions WITHOUT PARAMETERS.
IF using STATIC functions : Pass in a (..., void() ) function pointer
IF using MEMBER function : Pass in (..., void(T::* func)(), U* obj), where func is the Class, and obj is the Object calling it
(Why T and U for templating? Leftover code from Derivative classes calling Base Class function. It can probably? still be applied,
though I do not recommend it.)

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// This just adds a function to an already existing event
void AddFunctionToEvent(const std::string& name, std::string identifier, Trigger t, ...)

REFER TO ABOVE - Similar to Create Event, except name is used to determine which Event to add this Function to.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Changes Key that triggers the Event
void ChangeEventKey(const std::string& name, unsigned int keycode);

REFER TO ABOVE - Similar to the above, except it changes the keycode for the Event.
THIS WILL AFFECT ALL FUNCTIONS WITHIN THE EVENT - THEY WILL ONLY BE CALLED BY THIS KEYCODE.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Removes the Event from Registered Events
void RemoveEvent(const std::string& name);
REFER TO ABOVE - This removes an Event completely.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Removes a function attached to an Event
// Reminder to use this if you attach a member function to an Event
void RemoveAttachedFunction(const std::string& name, std::string identifier);
REFER TO ABOVE - This removes a specific Function from an Event.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Gets the Function that the Event runs
// Returns nullptr if unable to find
INPUT_EVENT GetInputEvent(const std::string& name, std::string identifier);
REFER TO ABOVE - Gets the actual Function Pointer to the std::function(void()). Dunno why you'd have this.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// All keys are now registered
void ALL_THE_KEYS();
IGNORES EVENTS. Assumes all Keys are Registered to an Event even though they do not have one.
Consider this as "I DO NOT CARE ABOUT EVENT SYSTEMS ANYMORE."

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// All keys are now deregistered
void NO_KEYS();
I HIGHLY RECOMMEND YOU NOT USE THIS ONE EVER.
THIS WILL WIPE OUT EVENT CHECKING. PLEASE DO NOT USE UNLESS YOU LITERALLY DO NOT CARE ABOUT
THE EVENTS.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx





/*******************************************************************************************/
                                         SI_MOUSEPOS
/*******************************************************************************************/

[ InputVec2 ]
Placeholder for vector 2s.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Updates Mouse Position, must run once to refresh mouse Positions
bool Update(float dt = 0);

Pretty standard.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Get Mouse Position Directly
POINT operator()();

REFER BELOW GetMousePos().

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Get Mouse Position - (0,0) Begins in Bottom-Left Corner
POINT GetMousePos();

Gets the Mouse Position based on the Screen Position. Origin is at Bottom Left.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Get Mouse Position - (0,0) Begins in Top-Left Corner
POINT GetOriginalMousePos();

Gets the Mouse position based on Screen Position. Origin is at Top Left.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Get Mouse Movement vector - (0,0) Begins in Bottom-Left Corner
POINT GetMouseDragVec();

Gets the Mouse Movement Vector. Depends on Update. The more frames, the more accurate.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Toggles if cursor is viewable or not
bool ToggleCursorVisible();

As it says on the tin.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Sets If can view cursor
void SetCursorVisible(bool show);

Do you really need to figure this out?

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Sets Cursor image - .cur or .ani files only
void LoadCursorImage(std::string name);

Loads a Cursor Image with its filename. DO REMEMBER TO PUT IN A PROPER FILEPATH.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Returns Client Rect Size in pixels
POINT GetClientRectSize();

Gets the Window Size.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Gets the cursor's position in the client rect from 0.0 to 1.0 in xy coordinates, (0,0) Begins in Bottom-Left Corner
InputVec2 GetRelativeLocation();

Gets a relative location or percetage of where the Cursor is in the Screen. From (0,0) to (1,1)

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



/*******************************************************************************************/
                                         SI_Controller
/*******************************************************************************************/
// Might need to rework

/*******************************************************************************************/
[ GamePad Key ]

SystemInput_ns::GAMEPAD_...

This is used to get the Controller Button you want to check.
DOES NOT INCLUDE TRIGGER BUTTONS.

[ ControllerVec2 ]
Placeholder for Vector 2.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Resets all Controllers
void ResetController(bool allowController = false);

Resets and Searches for Controllers again. Use this to determine if Controllers should be checked again
OR if you want to stop Controller Support.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Checks directly if a button is down on the controller
bool CTRLButtonDown(unsigned int buttonDown);

Not Event Based. Checks directly from Hardware.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Checks if button is presssed
// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
bool GetIfButtonPress(unsigned int buttonPressed);
// Checks if button is held
// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
bool GetIfButtonHeld(unsigned int buttonPressed);
// Checks if button is released
// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
bool GetIfButtonRelease(unsigned int buttonPressed);

EVENT ONLY checks. Checks the button only if it is already registered as an Event.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Enables all buttons
void ALLBUTTONS();

Enables all buttons for checking. Same as KeyPress.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Left Trigger = 0, Right Trigger = 1, all other values default to Left
float GetTrigger(int lr = 0);

Gets Left or Right Trigger, and how hard it's being pressed.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Left Analog Stick = 0, Right Analog Stick = 1, all other values default to Left
ControllerVec2 GetAnalog(int lr = 0);

Gets the Left or Right Analog Stick, and which direction it's going in.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Creates new Event (Member Functions)
void CreateNewEvent(std::string name, unsigned int button, CTRLTrigger trig, ...)

Adds an Event to the controller. Same as Keypress's Event system, except without an identifier.
This means that an Event is tied to one Function, unlike KeyPress where an Event can be tied
to multiple Functions.

... MAY REFER TO EITHER STATIC OR MEMBER FUNCTIONS.
BOTH REQUIRE void() functions WITHOUT PARAMETERS.
IF using STATIC functions : Pass in a (..., void() ) function pointer
IF using MEMBER function : Pass in (..., void(T::* func)(), U* obj), where func is the Class, and obj is the Object calling it
(Why T and U for templating? Leftover code from Derivative classes calling Base Class function. It can probably? still be applied,
though I do not recommend it.)


xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Creates new Trigger Event (Member Functions)
// lr -> Left Trigger = 0, Right Trigger = 1
void CreateNewTriggerEvent(std::string name, unsigned int lr, void(T::* func)(float), U* obj)

Creates an Event Specifically for Triggers.
NOTE:
Functions must take in a float - This is the Trigger Value.

REFER TO ABOVE.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Creates new Analog Event (Member Functions)
// lr -> Left Analog Stick = 0, Right Analog Stick = 1
void CreateNewAnalogEvent(std::string name, unsigned int lr, void(T::* func)(float, float), U* obj)*

Creates an Event Specifically for Triggers.
NOTE:
Functions must take in 2 floats - This is the Analog Stick Value.

REFER TO ABOVE.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Changes Key that triggers the Event
void ChangeEventKey(const std::string& name, unsigned int button);

Changes the key to run an Event. You cannot change Triggers or Analogs currently.
May add if necessary.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Removes the Event from Registered Events
// Do remove the Event IF you are using member functions and it is about to be destroyed
void RemoveEvent(const std::string& name);

Removes a Button Event.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Removes the Event from Registered Trigger Events
// Do remove the Event IF you are using member functions and it is about to be destroyed
void RemoveTriggerEvent(const std::string& name);

Removes a Trigger Event.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Removes the Event from Registered Analog Events
// Do remove the Event IF you are using member functions and it is about to be destroyed
void RemoveAnalogEvent(const std::string& name);

Removes an Analog Event.

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

bool Update(float dt = 0);

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


/*******************************************************************************************/


/*******************************************************************************************/
                                         EXAMPLE USE CASE
/*******************************************************************************************/

/*******************************************************************************************/

class testing
{
	void privateTest1()							{ std::cout << "PRIVATE BUTTON" << std::endl; }
	void privateTest2(float out)				{ std::cout << "PRIVATE TRIGGER: " << out << std::endl; }
	void privateTest3(float out, float out2)	{ std::cout << "PRIVATE ANALOG: " << out << " " << out2 << std::endl; }
public:
	void publicTest1(SystemInput::SystemKeyPress& t)
	{
		t.CreateNewEvent		    ("1", SystemInput::IKEY_A, "INTERNAL_TEST", SystemInput::OnPress, &testing::privateTest1, this);
	}
	void publicTest2(SystemInput::SystemController& t)
	{
		t.CreateNewEvent		    ("2", SystemInput::GAMEPAD_CIRCLE, SystemInput::OnCTRLPress, &testing::privateTest1, this);
		t.CreateNewTriggerEvent	("3", 1, &testing::privateTest2, this);
		t.CreateNewAnalogEvent	("4", 1, &testing::privateTest3, this);
	}
};

void AnalogTest(float x, float y)
{
	std::cout << "ANALOG: " << x << " " << y << std::endl;
}

void TriggerTest(float dt)
{
	std::cout << "TRIGGER: " << dt << std::endl;
}

void ButtonTest()
{
	std::cout << "BUTTON" << std::endl;
}

int main()
{
	// ONLY IF USING GETKEYPRESS etc.
	//GLOBAL_SYSTEM_KEYPRESS.ALL_THE_KEYS();
	//GLOBAL_SYSTEM_CONTROLLER.ALLBUTTONS();

	GLOBAL_SYSTEM_CONTROLLER.ResetController(true);

	
	// MEMBER FUNCTION TESTING
	testing test;
	// NOT NECESSARY TO PASS IN, IF CAN ACCESS FROM INSIDE CLASS MANUALLY
	test.publicTest1(GLOBAL_SYSTEM_KEYPRESS);
	test.publicTest2(GLOBAL_SYSTEM_CONTROLLER);

	// STATIC FUNCTION TESTING
	GLOBAL_SYSTEM_KEYPRESS.CreateNewEvent("5", SystemInput::IKEY_A, "INTERNAL_TEST", SystemInput::OnPress, ButtonTest);

	GLOBAL_SYSTEM_CONTROLLER.CreateNewEvent("6", SystemInput::GAMEPAD_CIRCLE, SystemInput::OnCTRLPress, ButtonTest);
	GLOBAL_SYSTEM_CONTROLLER.CreateNewTriggerEvent("7", 0, TriggerTest);
	GLOBAL_SYSTEM_CONTROLLER.CreateNewAnalogEvent("8", 0, AnalogTest);

	while (1)
	{
		GLOBAL_SYSTEM_KEYPRESS.Update();
		GLOBAL_SYSTEM_MOUSEPOS.Update();
		GLOBAL_SYSTEM_CONTROLLER.Update();

		// Mouse Position
		POINT point = GLOBAL_SYSTEM_MOUSEPOS.GetMousePos();
		std::cout << "MOUSE POSITION: " << point.x << " " << point.y << std::endl;

		if (GLOBAL_SYSTEM_CONTROLLER.CTRLButtonDown(SystemInput::GAMEPAD_CROSS))
			break;
		if (GLOBAL_SYSTEM_KEYPRESS.GetKeyStateHold(SystemInput::IKEY_X))
			break;

	}

	return 0;
}






/*******************************************************************************************/