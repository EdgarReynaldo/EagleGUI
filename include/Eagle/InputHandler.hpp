
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file InputHandler.hpp
 * @brief The main interface for all state based input in Eagle
 *
 * All state based input is formed by an event driven state machine. State based input will only work if you call
 * @ref EagleInputHandler::HandleInputEvent or @ref EagleSystem::WaitForSystemEventAndUpdateState .
 */

#ifndef InputHandler_H
#define InputHandler_H



#include <string>

#include "Eagle/Events.hpp"
#include "Eagle/Timer.hpp"



class EagleGraphicsContext;

/**! @enum INPUTSRC
 *   @brief An enumeration to distinguish input sources. TODO : Multiple mice? Keyboards?
 */

enum INPUTSRC {
   KB  = 0,///< Keyboard input
   MS  = 1,///< Mouse input
   JS1 = 2,///< Joystick 1 input
   JS2 = 3,///< Joystick 2 input
   JS3 = 4,///< Joystick 3 input
   JS4 = 5,///< Joystick 4 input
   JS5 = 6,///< Joystick 5 input
   JS6 = 7,///< Joystick 6 input
   JS7 = 8,///< Joystick 7 input
   JS8 = 9,///< Joystick 8 input
   NUM_INPUT_SRCS = 10
};



extern const char* input_source_str[NUM_INPUT_SRCS];///< Strings corresponding to INPUTSRC



/**! @enum INPUTSTATE
 *   @brief Bit flags for distinguishing types of input
 */

enum INPUTSTATE {
   PRESS    = 1,///< This button was pressed just now
   RELEASE  = 2,///< This button was just released
   HELD     = 4,///< This button is being held
   OPEN     = 8,///< This button is still open
   DBLCLICK = 17,///< This button has been clicked twice quickly.  [16 | 1] (PRESS)
   NUM_INPUT_STATES = 5
};



extern const char* input_state_str[NUM_INPUT_STATES];///< Strings corresponding to input states


/**! @enum MOUSEBUTTON
 *   @brief Constants to distinguish mouse buttons
 */

enum MOUSEBUTTON {
   LMB  = 1,///< Left mouse button
   RMB  = 2,///< Right mouse button
   MMB  = 3,///< Middle mouse button
   EMB1 = 4,///< Extra mouse button 1
   EMB2 = 5,///< Extra mouse button 2
   EMB3 = 6,///< Extra mouse button 3
   EMB4 = 7,///< Extra mouse button 4
   EMB5 = 8 ///< Extra mouse button 5
};

/**! @enum JOYSTICKBUTTON
 *   @brief Joystick button 1 thru 16, zero based (subtract 1 from N)
 */

enum JOYSTICKBUTTON {
   JSBTN1  = 0,
   JSBTN2  = 1,
   JSBTN3  = 2,
   JSBTN4  = 3,
   JSBTN5  = 4,
   JSBTN6  = 5,
   JSBTN7  = 6,
   JSBTN8  = 7,
   JSBTN9  = 8,
   JSBTN10 = 9,
   JSBTN11 = 10,
   JSBTN12 = 11,
   JSBTN13 = 12,
   JSBTN14 = 13,
   JSBTN15 = 14,
   JSBTN16 = 15
};

/**! @enum EAGLE_KEYCODES
 *   @brief Eagle keycodes borrowed from Allegro 5 keycodes for convenience, ease in porting 
 *          TODO : Make a more permanent solution once other system drivers are in place.
 */

enum EAGLE_KEYCODES {
   EAGLE_KEY_A		= 1,
   EAGLE_KEY_B		= 2,
   EAGLE_KEY_C		= 3,
   EAGLE_KEY_D		= 4,
   EAGLE_KEY_E		= 5,
   EAGLE_KEY_F		= 6,
   EAGLE_KEY_G		= 7,
   EAGLE_KEY_H		= 8,
   EAGLE_KEY_I		= 9,
   EAGLE_KEY_J		= 10,
   EAGLE_KEY_K		= 11,
   EAGLE_KEY_L		= 12,
   EAGLE_KEY_M		= 13,
   EAGLE_KEY_N		= 14,
   EAGLE_KEY_O		= 15,
   EAGLE_KEY_P		= 16,
   EAGLE_KEY_Q		= 17,
   EAGLE_KEY_R		= 18,
   EAGLE_KEY_S		= 19,
   EAGLE_KEY_T		= 20,
   EAGLE_KEY_U		= 21,
   EAGLE_KEY_V		= 22,
   EAGLE_KEY_W		= 23,
   EAGLE_KEY_X		= 24,
   EAGLE_KEY_Y		= 25,
   EAGLE_KEY_Z		= 26,

   EAGLE_KEY_0		= 27,
   EAGLE_KEY_1		= 28,
   EAGLE_KEY_2		= 29,
   EAGLE_KEY_3		= 30,
   EAGLE_KEY_4		= 31,
   EAGLE_KEY_5		= 32,
   EAGLE_KEY_6		= 33,
   EAGLE_KEY_7		= 34,
   EAGLE_KEY_8		= 35,
   EAGLE_KEY_9		= 36,

   EAGLE_KEY_PAD_0		= 37,
   EAGLE_KEY_PAD_1		= 38,
   EAGLE_KEY_PAD_2		= 39,
   EAGLE_KEY_PAD_3		= 40,
   EAGLE_KEY_PAD_4		= 41,
   EAGLE_KEY_PAD_5		= 42,
   EAGLE_KEY_PAD_6		= 43,
   EAGLE_KEY_PAD_7		= 44,
   EAGLE_KEY_PAD_8		= 45,
   EAGLE_KEY_PAD_9		= 46,

   EAGLE_KEY_F1		= 47,
   EAGLE_KEY_F2		= 48,
   EAGLE_KEY_F3		= 49,
   EAGLE_KEY_F4		= 50,
   EAGLE_KEY_F5		= 51,
   EAGLE_KEY_F6		= 52,
   EAGLE_KEY_F7		= 53,
   EAGLE_KEY_F8		= 54,
   EAGLE_KEY_F9		= 55,
   EAGLE_KEY_F10		= 56,
   EAGLE_KEY_F11		= 57,
   EAGLE_KEY_F12		= 58,

   EAGLE_KEY_ESCAPE	   = 59,
   EAGLE_KEY_TILDE		= 60,
   EAGLE_KEY_MINUS		= 61,
   EAGLE_KEY_EQUALS	   = 62,
   EAGLE_KEY_BACKSPACE	= 63,
   EAGLE_KEY_TAB		   = 64,
   EAGLE_KEY_OPENBRACE	= 65,
   EAGLE_KEY_CLOSEBRACE	= 66,
   EAGLE_KEY_ENTER		= 67,
   EAGLE_KEY_SEMICOLON	= 68,
   EAGLE_KEY_QUOTE		= 69,
   EAGLE_KEY_BACKSLASH	= 70,
   EAGLE_KEY_BACKSLASH2	= 71, /* DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards" */
   EAGLE_KEY_COMMA		= 72,
   EAGLE_KEY_FULLSTOP	= 73,
   EAGLE_KEY_SLASH		= 74,
   EAGLE_KEY_SPACE		= 75,

   EAGLE_KEY_INSERT	= 76,
   EAGLE_KEY_DELETE	= 77,
   EAGLE_KEY_HOME		= 78,
   EAGLE_KEY_END		= 79,
   EAGLE_KEY_PGUP		= 80,
   EAGLE_KEY_PGDN		= 81,
   EAGLE_KEY_LEFT		= 82,
   EAGLE_KEY_RIGHT   = 83,
   EAGLE_KEY_UP		= 84,
   EAGLE_KEY_DOWN		= 85,

   EAGLE_KEY_PAD_SLASH	   = 86,
   EAGLE_KEY_PAD_ASTERISK	= 87,
   EAGLE_KEY_PAD_MINUS	   = 88,
   EAGLE_KEY_PAD_PLUS	   = 89,
   EAGLE_KEY_PAD_DELETE	   = 90,
   EAGLE_KEY_PAD_ENTER	   = 91,

   EAGLE_KEY_PRINTSCREEN	= 92,
   EAGLE_KEY_PAUSE		   = 93,

   EAGLE_KEY_ABNT_C1	   = 94,
   EAGLE_KEY_YEN		   = 95,
   EAGLE_KEY_KANA		   = 96,
   EAGLE_KEY_CONVERT	   = 97,
   EAGLE_KEY_NOCONVERT	= 98,
   EAGLE_KEY_AT		   = 99,
   EAGLE_KEY_CIRCUMFLEX	= 100,
   EAGLE_KEY_COLON2	   = 101,
   EAGLE_KEY_KANJI		= 102,

   EAGLE_KEY_PAD_EQUALS	= 103,	/* MacOS X */
   EAGLE_KEY_BACKQUOTE	= 104,	/* MacOS X */
   EAGLE_KEY_SEMICOLON2	= 105,	/* MacOS X -- TODO: ask lillo what this should be */
   EAGLE_KEY_COMMAND	   = 106,	/* MacOS X */
   EAGLE_KEY_UNKNOWN    = 107,

   /** All codes up to before EAGLE_KEY_MODIFIERS can be freely
    * assignedas additional unknown keys, like various multimedia
    * and application keys keyboards may have.
    */

   EAGLE_KEY_MODIFIERS	= 215,

   EAGLE_KEY_LSHIFT	   = 215,
   EAGLE_KEY_RSHIFT	   = 216,
   EAGLE_KEY_LCTRL	   = 217,
   EAGLE_KEY_RCTRL	   = 218,
   EAGLE_KEY_ALT		   = 219,
   EAGLE_KEY_ALTGR	   = 220,
   EAGLE_KEY_LWIN		   = 221,
   EAGLE_KEY_RWIN		   = 222,
   EAGLE_KEY_MENU		   = 223,
   EAGLE_KEY_SCROLLLOCK = 224,
   EAGLE_KEY_NUMLOCK	   = 225,
   EAGLE_KEY_CAPSLOCK	= 226,

   EAGLE_KEY_MAX
};

/**! @enum EAGLE_EXTENDED_KEYCODES
 *   @brief Additional key codes for the states of the shift, ctrl, and alt modifier keys,
 *          as well as their possible combinations. 
 */

enum EAGLE_EXTENDED_KEYCODES {
   EAGLE_KEY_NONE               = 0,///< No key
   EAGLE_KEY_ANY_SHIFT          = EAGLE_KEY_MAX + 0,///< Any shift key
   EAGLE_KEY_ANY_CTRL           = EAGLE_KEY_MAX + 1,///< Any control key
   EAGLE_KEY_ANY_ALT            = EAGLE_KEY_MAX + 2,///< Any alt key

   EAGLE_KEY_NO_SHIFT           = EAGLE_KEY_MAX + 3,///< Any mod key but shift
   EAGLE_KEY_NO_CTRL            = EAGLE_KEY_MAX + 4,///< Any mod key but control
   EAGLE_KEY_NO_ALT             = EAGLE_KEY_MAX + 5,///< Any mod key but alt

   EAGLE_KEY_NO_MOD             = EAGLE_KEY_MAX + 6,///< No mod key
   EAGLE_KEY_ONLY_SHIFT         = EAGLE_KEY_MAX + 7,///< Only shift mod (no ctrl or alt)
   EAGLE_KEY_ONLY_CTRL          = EAGLE_KEY_MAX + 8,///< Only control mod (no shift or alt)
   EAGLE_KEY_ONLY_ALT           = EAGLE_KEY_MAX + 9,///< Only alt mod (no shift or ctrl)

   EAGLE_KEY_ONLY_SHIFT_CTRL    = EAGLE_KEY_MAX + 10,///< Only shift and control mods 
   EAGLE_KEY_ONLY_SHIFT_ALT     = EAGLE_KEY_MAX + 11,///< Only shift and alt mods
   EAGLE_KEY_ONLY_CTRL_ALT      = EAGLE_KEY_MAX + 12,///< Only control and alt mods

   EAGLE_KEY_STATE_EXTENDED_MAX = EAGLE_KEY_MAX + 13 ///< The absolute max number of keys
};

/**! @def JS_MAX_NUM_JOYSTICKS
 *   @brief The maximum number of joysticks tracked by eagle
 *
 *   @def JS_MAX_NUM_STICKS
 *   @brief The maximum number of sticks per joystick
 *
 *   @def JS_MAX_NUM_AXES
 *   @brief The maximum number of axes per stick
 *
 *   @def JS_MAX_NUM_BUTTONS
 *   @brief The maximum number of buttons per joystick tracked by eagle
 */

#define JS_MAX_NUM_JOYSTICKS 8
#define JS_MAX_NUM_STICKS 8
#define JS_MAX_NUM_AXES 4
#define JS_MAX_NUM_BUTTONS 16


/**! @def MS_MAX_NUM_BUTTONS
 *   @brief The maximum number of mouse buttons tracked by Eagle
 */

#define MS_MAX_NUM_BUTTONS 8


const char* keycode_to_name(int keycode);///< @param keycode 0 to EAGLE_KEY_STATE_EXTENDED_MAX
const char* mouse_button_to_name(int button);///< @param button 1 to 8, can use MOUSEBUTTON
const char* joy_button_to_name(int button);///< @param button 0 to 15, can use JSBTN1...

/**! @typedef VALUE_TO_NAME_FUNC
 *   @brief Typedef for a function that takes a value for a button and returns a name
 */

typedef const char* (*VALUE_TO_NAME_FUNC) (int);



///< Array of functions to redirect from an input source to a @ref VALUE_TO_NAME_FUNC
extern const VALUE_TO_NAME_FUNC value_to_name_func[NUM_INPUT_SRCS];

///< A floating point variable that determines the maximum duration a double click can occur in (smaller is faster)
extern float double_click_duration;


/**! @class EagleJoystickData
 *   @brief The basic data model for a joystick in Eagle
 */

class EagleJoystickData {

public :
   bool pluggedin;                                ///< True if this stick is plugged in, some sticks may have been removed
   void* joystick;                                ///< The actual joystick (depends on system driver and input driver)
   int num_sticks;                                ///< The number of sticks on this joystick
   int num_axes[JS_MAX_NUM_STICKS];               ///< An array of the number of axes for each stick
   int num_buttons;                               ///< The number of buttons on this joystick
   int buttonstates[JS_MAX_NUM_BUTTONS];          ///< Array of KEYSTATE bitfields
   float since_last_jspress[JS_MAX_NUM_BUTTONS];  ///< Array of times since last press for each button (tracks double clicks)
   float button_held_duration[JS_MAX_NUM_BUTTONS];///< Array of durations each button has been held for
   float axes[JS_MAX_NUM_STICKS][JS_MAX_NUM_AXES];///< 2D Array of floating point positions on each axis for each stick on this joystick
   
   EagleJoystickData();///< Empty constructor
   ///< TODO : Implement copy constructor
};



class Input;
class InputGroup;


/**! @class EagleInputHandler
 *   @brief The main class for dealing with initializing and reporting input
 */

class EagleInputHandler : public EagleObject , public EagleEventSource , public EagleEventListener {
private :
   
   virtual void PrivateInitializeJoystickInput()=0;///< Called by @ref InitializeJoystickInput
   virtual void PrivateInitializeKeyboardInput()=0;///< Called by @ref InitializeKeyboardInput
   virtual void PrivateInitializeMouseInput()=0;///< Called by @ref InitializeMouseInput
   virtual void PrivateInitializeTouchInput()=0;///< Called by @ref InitializeTouchInput

public :
   EagleInputHandler(std::string objclass = "EagleInputHandler" , std::string objname = "Nemo");///< Basic empty constructor
   virtual ~EagleInputHandler() {}///< Virtual destructor, as this is a base class for drivers to implement
   
   void InitializeInput();///< Initialize all input, called for you by your system driver
   
   void InitializeKeyboardInput();///< Initialize keyboard input, called for you by your system driver
   void InitializeMouseInput();///< Initialize mouse input, called for you by your system driver
   void InitializeJoystickInput();///< Initialize joystick input, called for you by your system driver
   void InitializeTouchInput();///< Initialize touch input, called for you by your system driver
   
   void ClearKeyState();///< Use if necessary when switching between displays and a key is stuck
   
   virtual int NumJoysticksInstalled()=0;///< Pure virtual function to get the number of joysticks installed, may change
   
   virtual void GetKeyboardState()=0;///< Pure virtual function to update the keyboard state, for backend input handlers
   virtual void GetMouseState()=0;///< Pure virtual function to update the mouse state, for backend input handlers
   virtual void GetJoystickStates()=0;///< Pure virtual function to update the joystick state, for backend input handlers
   virtual void GetTouchState()=0;///< Pure virtual function to update the touch state, for backend input handlers

   virtual void StartKeyboardEventHandler()=0;///< Pure virtual function to start the keyboard event handler (automatic)
   virtual void StartJoystickEventHandler()=0;///< Pure virtual function to start the joystick event handler (automatic)
   virtual void StartMouseEventHandler()=0;///< Pure virtual function to start the mouse event handler (automatic)
   virtual void StartTouchEventHandler()=0;///< Pure virtual function to start the touch event handler (automatic)



   /**! @fn HandleInputEvent <EagleEvent>
    *   @brief If you want to run the input handler manually, use this on every event you receive
    *          NOTE : This is called automatically by @ref EagleSystem::WaitForSystemEventAndUpdateState
    */
   void HandleInputEvent(EagleEvent ev);

   /**! @fn RecordInputPress <EagleEventHandler*,Input*> @fn RecordInputGroup<EagleEventHandler*,InputGroup*>
    *   Input recording functions for setting Inputs and InputGroups to key/mouse/joystick input
    *
    *   @ref RecordInputPress records the next single @ref Input object, storing it in the provided pointer. 
    *        @param input must be non-null.
    *   @ref RecordInputGroup records the next @ref InputGroup object, storing both the press, and any modifiers held.
    *        @param input_group must be non-NULL
    *   These are both blocking calls - they will wait forever until there is input 
    */
   void RecordInputPress(EagleEventHandler* queue , Input* input);// records first key press, blocks
   void RecordInputGroup(EagleEventHandler* queue , InputGroup* input_group);// records first key press along with any modifiers
};



/**

void InitializeJoysticks() {
   num_joysticks = al_get_num_joysticks();
   for (int i = 0 ; i < num_joysticks ; ++i) {
      joysticks[i].Initialize(al_get_joystick(i));
   }
}

void InitializeKeyboardInput() {
   for (int i = 0 ; i < EAGLE_KEY_STATE_EXTENDED_MAX ; ++i) {
      keystates[i] = OPEN;
   }
   for (int i = 0 ; i < EAGLE_KEY_MAX ; ++i) {
      keydown[i] = 0;
      key_held_duration[i] = 0.0f;
   }
}




*/

/** TODO : Wrap all input into sys->Input()->KB()->KeyPressed(EAGLE_KEY_A) */

extern EagleJoystickData joysticks[JS_MAX_NUM_JOYSTICKS];///< An array of @ref JS_MAX_NUM_JOYSTICKS joysticks for storing all joystick data
extern int num_joysticks;///< Number of joysticks attached

int get_joystick_n(void* joystick_source);///< Get the joystick number from the source pointer (TODO : What kind of pointer should this be?)

extern char keystates[EAGLE_KEY_STATE_EXTENDED_MAX];         ///< An array of KEYSTATE bitfields
extern bool keydown[EAGLE_KEY_MAX];                          ///< An array of key down states
extern float since_last_keypress[EAGLE_KEY_MAX];             ///< An array of durations since last key press per key
extern float key_held_duration[EAGLE_KEY_STATE_EXTENDED_MAX];///< An array of durations for which the key has been held

/** @var mouse_press @var mouse_release @var mouse_held @var mouse_open @var mouse_dblclick
 *  @brief Button bitfields representing the states of each mouse button
 *
 *  Ex.
 *  ```
 *      if (mouse_press & 1) {/// LMB pressed
 *  ```
 *
 *  See the shortcut functions for testing mouse states, such as 
 *  @ref ms_press , @ref ms_held , @ref ms_release , @ref ms_open , and @ref ms_dblclick
 */
extern int mouse_press;///< Button bitfield for mouse presses
extern int mouse_release;///< Button bitfield for mouse releases
extern int mouse_held;///< Button bitfield for mouse button holding
extern int mouse_open;///< Button bitfield for mouse button open state
extern int mouse_dblclick;///< Button bitfield for mouse button double click state, governed by @ref double_click_duration

extern int mouse_down;///< Button bitfield for mouse button down
extern bool mouse_on;///< True if the mouse is on a display

extern float msbtn_held_duration[MS_MAX_NUM_BUTTONS];///< Array of durations each mouse button has been held down
extern float since_last_mspress[MS_MAX_NUM_BUTTONS];///< Array of time elapsed since the last button press

/** @var mouse_x , @var mouse_y , @var mouse_w , @var mouse_z , 
 *  @brief These variables hold the current values for the mouse x , y , w , and z
 */

extern int mouse_x;
extern int mouse_y;
extern int mouse_w;
extern int mouse_z;

/** @var mouse_dx , @var mouse_dy , @var mouse_dw , @var mouse_dz
 *  These variables hold the difference in x, y, w, and z values for the mouse since the last event
 */

extern int mouse_dx;
extern int mouse_dy;
extern int mouse_dw;
extern int mouse_dz;
   
extern EagleGraphicsContext* last_display_read;///< A pointer to the last display read in input


void SetInputTimer(EagleTimer* timer);///< Use your own input timer

int ReadKey(EagleEventHandler* queue);///< Get a keycode from the keyboard. Blocking call.


/** @fn kb_press <int> , @fn kb_held <int> , @fn kb_release <int> , @fn kb_open <int> , @fn kb_dblclick <int>
 *  @brief These functions tell you the press, held, release, open, or double click states of the specified keycode.
 *  @param key Pass an @ref EAGLE_KEYCODES enumeration value to see if that key was in that state
 */

bool kb_press(int key);
bool kb_held(int key);
bool kb_release(int key);
bool kb_open(int key);
bool kb_dblclick(int key);

/** @fn ms_press <int> , @fn ms_held <int> , @fn ms_release <int> , @fn ms_open <int> , @fn ms_dblclick <int>
 *  @brief These functions tell you the press, held, release, open, or double click states of the specified keycode.
 *  @param btn Pass a @ref MOUSEBUTTON enumeration value like @ref LMB to see if that key was in that state
 */

bool ms_press(int btn);
bool ms_held(int btn);
bool ms_release(int btn);
bool ms_open(int btn);
bool ms_dblclick(int btn);

/** @fn js_press <int,int> , @fn js_held <int,int> , @fn js_release <int,int> , @fn js_open <int,int> , @fn js_dblclick <int,int>
 *  @brief These functions tell you the press, held, release, open, or double click states of the specified joystick and button.
 *  @param joy_num Pass a value from 0 to @ref JS_MAX_NUM_JOYSTICKS - 1 to see if that joystick was in that state
 *  @param btn Pass a @ref JOYSTICKBUTTON enumeration value like @ref JSBTN1 to see if that button was in that state
 */

bool js_press(int joy_num , int btn);
bool js_held(int joy_num , int btn);
bool js_release(int joy_num , int btn);
bool js_open(int joy_num , int btn);
bool js_dblclick(int joy_num , int btn);

/** @fn js1_press <int> , @fn js1_held <int> , @fn js1_release <int> , @fn js1_open <int> , @fn js1_dblclick <int>
 *  @fn js2_press <int> , @fn js2_held <int> , @fn js2_release <int> , @fn js2_open <int> , @fn js2_dblclick <int>
 *  @fn js3_press <int> , @fn js3_held <int> , @fn js3_release <int> , @fn js3_open <int> , @fn js3_dblclick <int>
 *  @fn js4_press <int> , @fn js4_held <int> , @fn js4_release <int> , @fn js4_open <int> , @fn js4_dblclick <int>
 *  @fn js5_press <int> , @fn js5_held <int> , @fn js5_release <int> , @fn js5_open <int> , @fn js5_dblclick <int>
 *  @fn js6_press <int> , @fn js6_held <int> , @fn js6_release <int> , @fn js6_open <int> , @fn js6_dblclick <int>
 *  @fn js7_press <int> , @fn js7_held <int> , @fn js7_release <int> , @fn js7_open <int> , @fn js7_dblclick <int>
 *  @fn js8_press <int> , @fn js8_held <int> , @fn js8_release <int> , @fn js8_open <int> , @fn js8_dblclick <int>
 *  @brief These functions tell you the press, held, release, open, or double click states of the specified joystick and button.
 *  @param btn Pass a @ref JOYSTICKBUTTON enumeration value like @ref JSBTN1 to see if that button was in that state for the 
 *         specified joystick.
 */

bool js1_press(int btn);
bool js1_held(int btn);
bool js1_release(int btn);
bool js1_open(int btn);
bool js1_dblclick(int btn);

bool js2_press(int btn);
bool js2_held(int btn);
bool js2_release(int btn);
bool js2_open(int btn);
bool js2_dblclick(int btn);

bool js3_press(int btn);
bool js3_held(int btn);
bool js3_release(int btn);
bool js3_open(int btn);
bool js3_dblclick(int btn);

bool js4_press(int btn);
bool js4_held(int btn);
bool js4_release(int btn);
bool js4_open(int btn);
bool js4_dblclick(int btn);

bool js5_press(int btn);
bool js5_held(int btn);
bool js5_release(int btn);
bool js5_open(int btn);
bool js5_dblclick(int btn);

bool js6_press(int btn);
bool js6_held(int btn);
bool js6_release(int btn);
bool js6_open(int btn);
bool js6_dblclick(int btn);

bool js7_press(int btn);
bool js7_held(int btn);
bool js7_release(int btn);
bool js7_open(int btn);
bool js7_dblclick(int btn);

bool js8_press(int btn);
bool js8_held(int btn);
bool js8_release(int btn);
bool js8_open(int btn);
bool js8_dblclick(int btn);





#endif // InputHandler_H



