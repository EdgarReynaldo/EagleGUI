
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef InputHandler_H
#define InputHandler_H



#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Eagle/Events.hpp"
#include "Eagle/Timer.hpp"



class EagleGraphicsContext;

enum INPUTSRC {
   KB = 0,
   MS = 1,
   JS1 = 2,
   JS2 = 3,
   JS3 = 4,
   JS4 = 5,
   JS5 = 6,
   JS6 = 7,
   JS7 = 8,
   JS8 = 9,
   NUM_INPUT_SRCS = 10
};

extern const char* input_source_str[NUM_INPUT_SRCS];

enum INPUTSTATE {
   PRESS    = 1,
   RELEASE  = 2,
   HELD     = 4,
   OPEN     = 8,
   DBLCLICK = 17,// 16 | 1 (PRESS)
   NUM_INPUT_STATES = 5
};

//extern const int NUM_INPUT_STATES;

extern const char* input_state_str[NUM_INPUT_STATES];

enum MOUSEBUTTON {
   LMB  = 1,
   RMB  = 2,
   MMB  = 3,
   EMB1 = 4,
   EMB2 = 5,
   EMB3 = 6,
   EMB4 = 7,
   EMB5 = 8
};

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

/** Eagle keycodes borrowed from Allegro 5 keycodes for convenience, ease in porting */

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

   EAGLE_KEY_ESCAPE	= 59,
   EAGLE_KEY_TILDE		= 60,
   EAGLE_KEY_MINUS		= 61,
   EAGLE_KEY_EQUALS	= 62,
   EAGLE_KEY_BACKSPACE	= 63,
   EAGLE_KEY_TAB		= 64,
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
   EAGLE_KEY_RIGHT		= 83,
   EAGLE_KEY_UP		= 84,
   EAGLE_KEY_DOWN		= 85,

   EAGLE_KEY_PAD_SLASH	= 86,
   EAGLE_KEY_PAD_ASTERISK	= 87,
   EAGLE_KEY_PAD_MINUS	= 88,
   EAGLE_KEY_PAD_PLUS	= 89,
   EAGLE_KEY_PAD_DELETE	= 90,
   EAGLE_KEY_PAD_ENTER	= 91,

   EAGLE_KEY_PRINTSCREEN	= 92,
   EAGLE_KEY_PAUSE		= 93,

   EAGLE_KEY_ABNT_C1	= 94,
   EAGLE_KEY_YEN		= 95,
   EAGLE_KEY_KANA		= 96,
   EAGLE_KEY_CONVERT	= 97,
   EAGLE_KEY_NOCONVERT	= 98,
   EAGLE_KEY_AT		= 99,
   EAGLE_KEY_CIRCUMFLEX	= 100,
   EAGLE_KEY_COLON2	= 101,
   EAGLE_KEY_KANJI		= 102,

   EAGLE_KEY_PAD_EQUALS	= 103,	/* MacOS X */
   EAGLE_KEY_BACKQUOTE	= 104,	/* MacOS X */
   EAGLE_KEY_SEMICOLON2	= 105,	/* MacOS X -- TODO: ask lillo what this should be */
   EAGLE_KEY_COMMAND	= 106,	/* MacOS X */
   EAGLE_KEY_UNKNOWN      = 107,

   /* All codes up to before EAGLE_KEY_MODIFIERS can be freely
    * assignedas additional unknown keys, like various multimedia
    * and application keys keyboards may have.
    */

   EAGLE_KEY_MODIFIERS	= 215,

   EAGLE_KEY_LSHIFT	= 215,
   EAGLE_KEY_RSHIFT	= 216,
   EAGLE_KEY_LCTRL	= 217,
   EAGLE_KEY_RCTRL	= 218,
   EAGLE_KEY_ALT		= 219,
   EAGLE_KEY_ALTGR	= 220,
   EAGLE_KEY_LWIN		= 221,
   EAGLE_KEY_RWIN		= 222,
   EAGLE_KEY_MENU		= 223,
   EAGLE_KEY_SCROLLLOCK = 224,
   EAGLE_KEY_NUMLOCK	= 225,
   EAGLE_KEY_CAPSLOCK	= 226,

   EAGLE_KEY_MAX
};

/** Additional key codes for the states of the shift, ctrl, and alt modifier keys,
   as well as their possible combinations. */

enum EAGLE_EXTENDED_KEYCODES {
   EAGLE_KEY_NONE               = 0,
   EAGLE_KEY_ANY_SHIFT          = EAGLE_KEY_MAX + 0,
   EAGLE_KEY_ANY_CTRL           = EAGLE_KEY_MAX + 1,
   EAGLE_KEY_ANY_ALT            = EAGLE_KEY_MAX + 2,

   EAGLE_KEY_NO_SHIFT           = EAGLE_KEY_MAX + 3,
   EAGLE_KEY_NO_CTRL            = EAGLE_KEY_MAX + 4,
   EAGLE_KEY_NO_ALT             = EAGLE_KEY_MAX + 5,

   EAGLE_KEY_NO_MOD             = EAGLE_KEY_MAX + 6,
   EAGLE_KEY_ONLY_SHIFT         = EAGLE_KEY_MAX + 7,
   EAGLE_KEY_ONLY_CTRL          = EAGLE_KEY_MAX + 8,
   EAGLE_KEY_ONLY_ALT           = EAGLE_KEY_MAX + 9,

   EAGLE_KEY_ONLY_SHIFT_CTRL    = EAGLE_KEY_MAX + 10,
   EAGLE_KEY_ONLY_SHIFT_ALT     = EAGLE_KEY_MAX + 11,
   EAGLE_KEY_ONLY_CTRL_ALT      = EAGLE_KEY_MAX + 12,

   EAGLE_KEY_STATE_EXTENDED_MAX = EAGLE_KEY_MAX + 13
};

#define JS_MAX_NUM_JOYSTICKS 8
#define JS_MAX_NUM_STICKS 8
#define JS_MAX_NUM_AXES 4
#define JS_MAX_NUM_BUTTONS 16

#define MS_MAX_NUM_BUTTONS 8


const char* keycode_to_name(int keycode);// 0 to EAGLE_KEY_STATE_EXTENDED_MAX
const char* mouse_button_to_name(int button);// 1 to 8, can use MOUSEBUTTON
const char* joy_button_to_name(int button);// 0 to 

typedef const char* (*VALUE_TO_NAME_FUNC) (int);

extern VALUE_TO_NAME_FUNC value_to_name_func[NUM_INPUT_SRCS];

extern float double_click_duration;



class EagleJoystickData {

public :
   bool pluggedin;
   void* joystick;
   int num_sticks;
   int num_axes[JS_MAX_NUM_STICKS];
   int num_buttons;
   int buttonstates[JS_MAX_NUM_BUTTONS];// array of KEYSTATE bitfields
   float since_last_jspress[JS_MAX_NUM_BUTTONS];
   float button_held_duration[JS_MAX_NUM_BUTTONS];
   float axes[JS_MAX_NUM_STICKS][JS_MAX_NUM_AXES];
   
   EagleJoystickData();
   
};



class EagleInputHandler {
private :
   
   virtual void PrivateInitializeJoystickInput()=0;// called by initializejoysticksinput
   virtual void PrivateInitializeKeyboardInput()=0;// called by initializekeyboardinput
   virtual void PrivateInitializeMouseInput()=0;// called by initializekeyboardinput
   virtual void PrivateInitializeTouchInput()=0;// called by initializekeyboardinput

public :
   EagleInputHandler() {}
   virtual ~EagleInputHandler() {}
   
   void InitializeInput();
   
   void InitializeKeyboardInput();
   void InitializeMouseInput();
   void InitializeJoystickInput();
   void InitializeTouchInput();
   
   void ClearKeyState();
   
   virtual int NumJoysticksInstalled()=0;
   
   virtual void GetKeyboardState()=0;
   virtual void GetMouseState()=0;
   virtual void GetJoystickStates()=0;
   virtual void GetTouchState()=0;

	virtual void RegisterKeyboardInput(EagleEventHandler* queue)=0;
	virtual void RegisterMouseInput   (EagleEventHandler* queue)=0;
	virtual void RegisterJoystickInput(EagleEventHandler* queue)=0;
	virtual void RegisterTouchInput   (EagleEventHandler* queue)=0;
	
	void RegisterInputs(EagleEventHandler* queue);

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


extern EagleJoystickData joysticks[JS_MAX_NUM_JOYSTICKS];
extern int num_joysticks;

int get_joystick_n(void* joystick_source);

extern char keystates[EAGLE_KEY_STATE_EXTENDED_MAX];// array of KEYSTATE bitfields
extern bool keydown[EAGLE_KEY_MAX];
extern float since_last_keypress[EAGLE_KEY_MAX];
extern float key_held_duration[EAGLE_KEY_STATE_EXTENDED_MAX];

extern int mouse_press;// button bitfield
extern int mouse_release;// button bitfield
extern int mouse_held;// button bitfield
extern int mouse_open;// button bitfield
extern int mouse_dblclick;// button bitfield
extern int mouse_down;
extern bool mouse_on;
extern float msbtn_held_duration[MS_MAX_NUM_BUTTONS];
extern float since_last_mspress[MS_MAX_NUM_BUTTONS];
extern int mouse_x;
extern int mouse_y;
extern int mouse_w;
extern int mouse_z;
extern int mouse_dx;
extern int mouse_dy;
extern int mouse_dw;
extern int mouse_dz;
   
extern EagleGraphicsContext* last_display_read;


void SetInputTimer(EagleTimer* timer);
void HandleInputEvent(EagleEvent ev);
int ReadKey(EagleEventHandler* queue);

bool kb_press(int key);
bool kb_held(int key);
bool kb_release(int key);
bool kb_open(int key);
bool kb_dblclick(int key);

bool ms_press(int btn);
bool ms_held(int btn);
bool ms_release(int btn);
bool ms_open(int btn);
bool ms_dblclick(int btn);

bool js_press(int joy_num , int btn);
bool js_held(int joy_num , int btn);
bool js_release(int joy_num , int btn);
bool js_open(int joy_num , int btn);
bool js_dblclick(int joy_num , int btn);

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

typedef bool (*INPUT_HANDLER)(int);

extern INPUT_HANDLER input_handlers[NUM_INPUT_SRCS][NUM_INPUT_STATES];
extern const char* input_func_text[NUM_INPUT_SRCS][NUM_INPUT_STATES];



class Input {

private :
   INPUTSRC src;
   INPUTSTATE state;
   int value;
   INPUT_HANDLER input_handler_func;


public :
   /** Constructors */
   Input();
   Input(INPUTSRC source , INPUTSTATE inputstate , int input_value);

   /** Reassignment */
   void AssignTo(INPUTSRC source , INPUTSTATE inputstate , int input_value);

   /** Present time evaluation of input (NOTE : won't change until UpdateInput is called) */
   operator bool() const;
   operator int() const;

   /** operator! can't be used for evaluation, it needs to return an InputGroup with NOT set */
   //  bool operator!() {return !(bool)(*this);} /*      (Implemented globally below)      */

   /** So you can compare Input objects */
   bool operator==(const Input& i);
   bool operator!=(const Input& i);

   /** Text output */
   friend std::ostream& operator<<(std::ostream& os , const Input& input);
   std::ostream& Shortname(std::ostream& os) const ;

   /** Getters */
   INPUTSRC Source() const {return src;}
   INPUTSTATE State()  const {return state;}
   int Value()  const {return value;}

};


inline Input input_key_press(int value)    {return Input(KB , PRESS    , value);}
inline Input input_key_release(int value)  {return Input(KB , RELEASE  , value);}
inline Input input_key_held(int value)     {return Input(KB , HELD     , value);}
inline Input input_key_open(int value)     {return Input(KB , OPEN     , value);}
inline Input input_key_dblclick(int value) {return Input(KB , DBLCLICK , value);}

inline Input input_mouse_press(int value)    {return Input(MS , PRESS    , value);}
inline Input input_mouse_release(int value)  {return Input(MS , RELEASE  , value);}
inline Input input_mouse_held(int value)     {return Input(MS , HELD     , value);}
inline Input input_mouse_open(int value)     {return Input(MS , OPEN     , value);}
inline Input input_mouse_dblclick(int value) {return Input(MS , DBLCLICK , value);}

inline Input input_jstk_press(int joy_num , int value)    {return Input(INPUTSRC(JS1 + joy_num) , PRESS    , value);}
inline Input input_jstk_release(int joy_num , int value)  {return Input(INPUTSRC(JS1 + joy_num) , RELEASE  , value);}
inline Input input_jstk_held(int joy_num , int value)     {return Input(INPUTSRC(JS1 + joy_num) , HELD     , value);}
inline Input input_jstk_open(int joy_num , int value)     {return Input(INPUTSRC(JS1 + joy_num) , OPEN     , value);}
inline Input input_jstk_dblclick(int joy_num , int value) {return Input(INPUTSRC(JS1 + joy_num) , DBLCLICK , value);}

inline Input input_jstk1_press(int value)    {return Input(JS1 , PRESS    , value);}
inline Input input_jstk1_release(int value)  {return Input(JS1 , RELEASE  , value);}
inline Input input_jstk1_held(int value)     {return Input(JS1 , HELD     , value);}
inline Input input_jstk1_open(int value)     {return Input(JS1 , OPEN     , value);}
inline Input input_jstk1_dblclick(int value) {return Input(JS1 , DBLCLICK , value);}

inline Input input_jstk2_press(int value)    {return Input(JS2 , PRESS    , value);}
inline Input input_jstk2_release(int value)  {return Input(JS2 , RELEASE  , value);}
inline Input input_jstk2_held(int value)     {return Input(JS2 , HELD     , value);}
inline Input input_jstk2_open(int value)     {return Input(JS2 , OPEN     , value);}
inline Input input_jstk2_dblclick(int value) {return Input(JS2 , DBLCLICK , value);}

inline Input input_jstk3_press(int value)    {return Input(JS3 , PRESS    , value);}
inline Input input_jstk3_release(int value)  {return Input(JS3 , RELEASE  , value);}
inline Input input_jstk3_held(int value)     {return Input(JS3 , HELD     , value);}
inline Input input_jstk3_open(int value)     {return Input(JS3 , OPEN     , value);}
inline Input input_jstk3_dblclick(int value) {return Input(JS3 , DBLCLICK , value);}

inline Input input_jstk4_press(int value)    {return Input(JS4 , PRESS    , value);}
inline Input input_jstk4_release(int value)  {return Input(JS4 , RELEASE  , value);}
inline Input input_jstk4_held(int value)     {return Input(JS4 , HELD     , value);}
inline Input input_jstk4_open(int value)     {return Input(JS4 , OPEN     , value);}
inline Input input_jstk4_dblclick(int value) {return Input(JS4 , DBLCLICK , value);}

inline Input input_jstk5_press(int value)    {return Input(JS5 , PRESS    , value);}
inline Input input_jstk5_release(int value)  {return Input(JS5 , RELEASE  , value);}
inline Input input_jstk5_held(int value)     {return Input(JS5 , HELD     , value);}
inline Input input_jstk5_open(int value)     {return Input(JS5 , OPEN     , value);}
inline Input input_jstk5_dblclick(int value) {return Input(JS5 , DBLCLICK , value);}

inline Input input_jstk6_press(int value)    {return Input(JS6 , PRESS    , value);}
inline Input input_jstk6_release(int value)  {return Input(JS6 , RELEASE  , value);}
inline Input input_jstk6_held(int value)     {return Input(JS6 , HELD     , value);}
inline Input input_jstk6_open(int value)     {return Input(JS6 , OPEN     , value);}
inline Input input_jstk6_dblclick(int value) {return Input(JS6 , DBLCLICK , value);}

inline Input input_jstk7_press(int value)    {return Input(JS7 , PRESS    , value);}
inline Input input_jstk7_release(int value)  {return Input(JS7 , RELEASE  , value);}
inline Input input_jstk7_held(int value)     {return Input(JS7 , HELD     , value);}
inline Input input_jstk7_open(int value)     {return Input(JS7 , OPEN     , value);}
inline Input input_jstk7_dblclick(int value) {return Input(JS7 , DBLCLICK , value);}

inline Input input_jstk8_press(int value)    {return Input(JS8 , PRESS    , value);}
inline Input input_jstk8_release(int value)  {return Input(JS8 , RELEASE  , value);}
inline Input input_jstk8_held(int value)     {return Input(JS8 , HELD     , value);}
inline Input input_jstk8_open(int value)     {return Input(JS8 , OPEN     , value);}
inline Input input_jstk8_dblclick(int value) {return Input(JS8 , DBLCLICK , value);}


/** How to implement the && || ! logic and chaining? With an object tree */

/** Tree class for logical grouping of Input evaluations and storage */

/** Trying to get a bool value from an empty input group will throw an exception, as
    returning true or false would be incorrect, since there's no logical test to evaluate */

class InputGroup {
private :
   bool all_apply;// True means this is operator AND - False means any apply (operator OR)
   bool is;       // False means is not (operator NOT)
   std::vector<Input>      inputs;
   std::vector<InputGroup> nodes;/** ??? This works somehow...? */
   mutable int node_level;

   void SetNodeLevel(int level) const;

   bool IS(bool basis) const {return (is?basis:!basis);}

   bool SameTypeAs(const InputGroup& ig) {return ((all_apply == ig.all_apply) && (is == ig.is));}

   /** TODO : Would equivalence operators be useful?
       TODO : InputGroups would probably have to be sorted first */
   bool operator==(const InputGroup& ig) const {(void)ig;return false;}

public :
   /** Constructors */
   InputGroup();
   InputGroup(const Input& i);
   InputGroup(const InputGroup& ig);
   explicit InputGroup(bool use_and , bool use_is , const Input& i);
   explicit InputGroup(bool use_and , bool use_is , const InputGroup& ig);

   /** Vector like behavior */
   void        clear();
   InputGroup& push_back(const Input& i);
   InputGroup& push_back(const InputGroup& ig);

   /** Logical grouping behavior */ /* TODO : Check that grouping of similar types groups inputs and groups */
   InputGroup operator&&(const InputGroup& ig) const ;
   InputGroup operator||(const InputGroup& ig) const ;
   InputGroup operator!() const ;

   /** Evaluation - Don't evaluate empty input groups! */
   operator bool() const;
   /* If it makes you feel better to call a function to evaluate the expression instead
      of just using "if (InputGroup object) {...}", then here you go : */
   bool operator()() const {return (bool)(*this);}

   /** Small utility function for checking whether the input is from a pointer source - it
         checks whether the input is from the mouse or joystick as well as being a button click.
         As long as there is at least one mouse or joystick click input, this will return true. */
   bool PointerInput() const;
   
   bool HasClickInput() const;
   Input FindClickInput() const;// returns input_key_press(EAGLE_KEY_NONE) if there is no click input in the group
   
   /** Text logging */
   friend std::ostream& operator<<(std::ostream& os , const InputGroup& ig);
   std::ostream& ShowLogic(std::ostream& os) const ;
   
   void WriteLines(std::vector<std::string>* strvec);

};

/** Global operators to create InputGroup objects from logically joined Inputs */
InputGroup operator&&(const Input& i1 , const Input& i2);
InputGroup operator||(const Input& i1 , const Input& i2);
InputGroup operator!(const Input& i);

InputGroup operator&&(const Input& i , const InputGroup& ig);
InputGroup operator||(const Input& i , const InputGroup& ig);


bool AnyInputPressed(Input* store);
bool NonModInputPressed(Input* store);
bool ModifierHeld(Input* store);


/** Input recording functions for setting Inputs and InputGroups to key/mouse/joystick input */
/** These are both blocking calls - they will wait forever until there is input */
void RecordInputPress(EagleEventHandler* queue , Input* input);// records first key press, blocks
void RecordInputGroup(EagleEventHandler* queue , InputGroup* input_group);// records first key press along with any modifiers



class InputAssignment {
private :
   typedef std::map<std::string , InputGroup> TASKINPUTMAP;
   typedef TASKINPUTMAP::iterator TIMIT;

   TASKINPUTMAP task_input_map;
//   std::map<std::string , InputGroup> task_input_map;

public :
   InputAssignment() : task_input_map() {}

   /** Should support some map features, like [], should allow for testing the input from the task name */
   void clear();
   InputGroup& operator[](const std::string& str);
   bool Activated(const std::string& str);/** TODO : Is there a better function name than Activated? */
};

/** Usage example
void AssignPlayerDefaults(InputAssignment& player) {
   player.clear();
   player["Punch"]     = input_key_press(KEY_ONLY_SHIFT);
   player["Kick"]      = input_key_press(KEY_ONLY_ALT);
   player["Jump"]      = input_key_press(KEY_UP);
   player["Crouch"]    = input_key_held(KEY_DOWN);
   player["MoveLeft"]  = input_key_held(KEY_LEFT);
   player["MoveRight"] = input_key_held(KEY_RIGHT);
}

InputAssignment Player;
AssignPlayerDefaults(Player);

if (Player.Activated("Punch")) {...}
if (player["Punch"]) {...}  // This should work as well, [] returns an InputGroup& and if () should evaluate it
*/



#endif // InputHandler_H








