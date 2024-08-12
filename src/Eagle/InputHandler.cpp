
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 *  @file InputHandler.cpp
 *  @brief Implementation of our input handler functions and definitions
 */



#include "Eagle/InputHandler.hpp"
#include "Eagle/Input.hpp"
#include "Eagle/InputGroup.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/GraphicsContext.hpp"



const char* input_source_str[NUM_INPUT_SRCS] = {
   "KB",
   "MS",
   "JS1",
   "JS2",
   "JS3",
   "JS4",
   "JS5",
   "JS6",
   "JS7",
   "JS8"
};

//const int NUM_INPUT_STATES = 5;

const char* input_state_str[NUM_INPUT_STATES] = {
   "PRESS",
   "RELEASE",
   "HELD",
   "OPEN",
   "DBLCLICK"
};

const char* eagle_keycode_to_name(int keycode) {
   /* Mercilessly borrowed from Allegro 5 */
   EAGLE_ASSERT(keycode >= 0 && keycode < EAGLE_KEY_MAX);
   static const char* eagle_keyboard_common_names[EAGLE_KEY_MAX] =
   {
      "(none)",     "A",          "B",          "C",
      "D",          "E",          "F",          "G",
      "H",          "I",          "J",          "K",
      "L",          "M",          "N",          "O",
      "P",          "Q",          "R",          "S",
      "T",          "U",          "V",          "W",
      "X",          "Y",          "Z",          "0",
      "1",          "2",          "3",          "4",
      "5",          "6",          "7",          "8",
      "9",          "PAD 0",      "PAD 1",      "PAD 2",
      "PAD 3",      "PAD 4",      "PAD 5",      "PAD 6",
      "PAD 7",      "PAD 8",      "PAD 9",      "F1",
      "F2",         "F3",         "F4",         "F5",
      "F6",         "F7",         "F8",         "F9",
      "F10",        "F11",        "F12",        "ESCAPE",
      "KEY60",      "KEY61",      "KEY62",      "BACKSPACE",
      "TAB",        "KEY65",      "KEY66",      "ENTER",
      "KEY68",      "KEY69",      "BACKSLASH",  "KEY71",
      "KEY72",      "KEY73",      "KEY74",      "SPACE",
      "INSERT",     "DELETE",     "HOME",       "END",
      "PGUP",       "PGDN",       "LEFT",       "RIGHT",
      "UP",         "DOWN",       "PAD /",      "PAD *",
      "PAD -",      "PAD +",      "PAD DELETE", "PAD ENTER",
      "PRINTSCREEN","PAUSE",      "KEY94",      "KEY95",
      "KEY96",      "KEY97",      "KEY98",      "KEY99",
      "KEY100",     "KEY101",     "KEY102",     "PAD =",
      "KEY104",     "KEY105",     "KEY106",     "KEY107",
      "KEY108",     "KEY109",     "KEY110",     "KEY111",
      "KEY112",     "KEY113",     "KEY114",     "KEY115",
      "KEY116",     "KEY117",     "KEY118",     "KEY119",
      "KEY120",     "KEY121",     "KEY122",     "KEY123",
      "KEY124",     "KEY125",     "KEY126",     "KEY127",
      "KEY128",     "KEY129",     "KEY130",     "KEY131",
      "KEY132",     "KEY133",     "KEY134",     "KEY135",
      "KEY136",     "KEY137",     "KEY138",     "KEY139",
      "KEY140",     "KEY141",     "KEY142",     "KEY143",
      "KEY144",     "KEY145",     "KEY146",     "KEY147",
      "KEY148",     "KEY149",     "KEY150",     "KEY151",
      "KEY152",     "KEY153",     "KEY154",     "KEY155",
      "KEY156",     "KEY157",     "KEY158",     "KEY159",
      "KEY160",     "KEY161",     "KEY162",     "KEY163",
      "KEY164",     "KEY165",     "KEY166",     "KEY167",
      "KEY168",     "KEY169",     "KEY170",     "KEY171",
      "KEY172",     "KEY173",     "KEY174",     "KEY175",
      "KEY176",     "KEY177",     "KEY178",     "KEY179",
      "KEY180",     "KEY181",     "KEY182",     "KEY183",
      "KEY184",     "KEY185",     "KEY186",     "KEY187",
      "KEY188",     "KEY189",     "KEY190",     "KEY191",
      "KEY192",     "KEY193",     "KEY194",     "KEY195",
      "KEY196",     "KEY197",     "KEY198",     "KEY199",
      "KEY200",     "KEY201",     "KEY202",     "KEY203",
      "KEY204",     "KEY205",     "KEY206",     "KEY207",
      "KEY208",     "KEY209",     "KEY210",     "KEY211",
      "KEY212",     "KEY213",     "KEY214",     "LSHIFT",
      "RSHIFT",     "LCTRL",      "RCTRL",      "ALT",
      "ALTGR",      "LWIN",       "RWIN",       "MENU",
      "SCROLLLOCK", "NUMLOCK",    "CAPSLOCK"
   };
   return eagle_keyboard_common_names[keycode];
}

const char* keycode_to_name(int keycode) {
   static const char* extended_key_names[EAGLE_KEY_STATE_EXTENDED_MAX - EAGLE_KEY_MAX] = {
      "ANY_SHIFT",
      "ANY_CTRL",
      "ANY_ALT",
      "NO_SHIFT",
      "NO_CTRL",
      "NO_ALT",
      "NO_MOD",
      "ONLY_SHIFT",
      "ONLY_CTRL",
      "ONLY_ALT",
      "ONLY_SHIFT_CTRL",
      "ONLY_SHIFT_ALT",
      "ONLY_CTRL_ALT"
   };
   if (keycode == 0) {
      return "KEY_NONE";
   }
   else if ((keycode > 0) && (keycode < EAGLE_KEY_MAX)) {
      return eagle_keycode_to_name(keycode);
   }
   else if ((keycode >= EAGLE_KEY_MAX) && (keycode < EAGLE_KEY_STATE_EXTENDED_MAX)) {
      return extended_key_names[keycode - EAGLE_KEY_MAX];
   }
   return 0;
}

const char* mouse_button_to_name(int button) {
   static const char* mouse_button_names[MS_MAX_NUM_BUTTONS + 1] = {
      "NONE",
      "LMB",
      "RMB",
      "MMB",
      "EMB1",
      "EMB2",
      "EMB3",
      "EMB4",
      "EMB5"
   };
   return mouse_button_names[button];
}

const char* joy_button_to_name(int button) {
   static const char* joy_button_names[JS_MAX_NUM_BUTTONS] = {
      "JSBTN1",
      "JSBTN2",
      "JSBTN3",
      "JSBTN4",
      "JSBTN5",
      "JSBTN6",
      "JSBTN7",
      "JSBTN8",
      "JSBTN9",
      "JSBTN10",
      "JSBTN11",
      "JSBTN12",
      "JSBTN13",
      "JSBTN14",
      "JSBTN15",
      "JSBTN16"
   };
   return joy_button_names[button];
};



const VALUE_TO_NAME_FUNC value_to_name_func[NUM_INPUT_SRCS] = {
   keycode_to_name,
   mouse_button_to_name,
   joy_button_to_name,
   joy_button_to_name,
   joy_button_to_name,
   joy_button_to_name,
   joy_button_to_name,
   joy_button_to_name,
   joy_button_to_name,
   joy_button_to_name
};


const int key_state_mask = OPEN | PRESS | RELEASE | HELD | DBLCLICK;

float double_click_duration = 0.5;


EagleJoystickData::EagleJoystickData() :
      pluggedin(false),
      joystick(0),
      num_sticks(0),
      num_axes(),
      num_buttons(0),
      buttonstates(),
      button_held_duration(),
      axes()
{
   for (int i = 0 ; i < JS_MAX_NUM_STICKS ; ++i) {
      num_axes[i] = 0;
      for (int j = 0 ; j < JS_MAX_NUM_AXES ; ++j) {
         axes[i][j] = 0.0f;
      }
   }
   for (int i = 0 ; i < JS_MAX_NUM_BUTTONS ; ++i) {
      buttonstates[i] = OPEN;
      button_held_duration[i] = 0.0f;
   }
}





EagleJoystickData joysticks[JS_MAX_NUM_JOYSTICKS];
int num_joysticks = 0;




/// -----------------------------    globals    --------------------------------------------


int get_joystick_n(void* joystick_source) {
   for (int i = 0 ; i < num_joysticks ; ++i) {
      if (joysticks[i].joystick == joystick_source) {
         return i;
      }
   }
   return -1;
}



char keystates[EAGLE_KEY_STATE_EXTENDED_MAX];// array of KEYSTATE bitfields
bool keydown[EAGLE_KEY_MAX];
float since_last_keypress[EAGLE_KEY_MAX];
float key_held_duration[EAGLE_KEY_STATE_EXTENDED_MAX];

int mouse_press = 0;// button bitfield
int mouse_release = 0;// button bitfield
int mouse_held = 0;// button bitfield
int mouse_open = ~0;// button bitfield
int mouse_dblclick = 0;// button bitfield
int mouse_down = 0;
bool mouse_on = true;
float msbtn_held_duration[MS_MAX_NUM_BUTTONS];
float since_last_mspress[MS_MAX_NUM_BUTTONS];
int mouse_x = 0;
int mouse_y = 0;
int mouse_w = 0;
int mouse_z = 0;
int mouse_dx = 0;
int mouse_dy = 0;
int mouse_dw = 0;
int mouse_dz = 0;

EagleTimer* input_timer = 0;

///EagleGraphicsContext* last_display_read = 0;





void UpdateModKeys() {
   keystates[EAGLE_KEY_ANY_SHIFT] = keystates[EAGLE_KEY_LSHIFT] | keystates[EAGLE_KEY_RSHIFT];
   keystates[EAGLE_KEY_ANY_CTRL] = keystates[EAGLE_KEY_LCTRL] | keystates[EAGLE_KEY_RCTRL];
   keystates[EAGLE_KEY_ANY_ALT] = keystates[EAGLE_KEY_ALT] | keystates[EAGLE_KEY_ALTGR];
   
   keystates[EAGLE_KEY_NO_SHIFT] = ~keystates[EAGLE_KEY_ANY_SHIFT] & key_state_mask;
   keystates[EAGLE_KEY_NO_CTRL] = ~keystates[EAGLE_KEY_ANY_CTRL] & key_state_mask;
   keystates[EAGLE_KEY_NO_ALT] = ~keystates[EAGLE_KEY_ANY_ALT] & key_state_mask;
   
   keystates[EAGLE_KEY_NO_MOD] = keystates[EAGLE_KEY_NO_SHIFT] & keystates[EAGLE_KEY_NO_CTRL] & keystates[EAGLE_KEY_NO_ALT];
   
   keystates[EAGLE_KEY_ONLY_SHIFT] = keystates[EAGLE_KEY_ANY_SHIFT] & keystates[EAGLE_KEY_NO_CTRL] & keystates[EAGLE_KEY_NO_ALT];
   keystates[EAGLE_KEY_ONLY_CTRL] = keystates[EAGLE_KEY_NO_SHIFT] & keystates[EAGLE_KEY_ANY_CTRL] & keystates[EAGLE_KEY_NO_ALT];
   keystates[EAGLE_KEY_ONLY_ALT] = keystates[EAGLE_KEY_NO_SHIFT] & keystates[EAGLE_KEY_NO_CTRL] & keystates[EAGLE_KEY_ANY_ALT];
   
   keystates[EAGLE_KEY_ONLY_SHIFT_CTRL] = keystates[EAGLE_KEY_ANY_SHIFT] & keystates[EAGLE_KEY_ANY_CTRL] & keystates[EAGLE_KEY_NO_ALT];
   keystates[EAGLE_KEY_ONLY_SHIFT_ALT] = keystates[EAGLE_KEY_ANY_SHIFT] & keystates[EAGLE_KEY_NO_CTRL] & keystates[EAGLE_KEY_ANY_ALT];
   keystates[EAGLE_KEY_ONLY_CTRL_ALT] = keystates[EAGLE_KEY_NO_SHIFT] & keystates[EAGLE_KEY_ANY_CTRL] & keystates[EAGLE_KEY_ANY_ALT];
}



void SetInputTimer(EagleTimer* timer) {
   input_timer = timer;
}



int ReadKey(EagleEventHandler* queue) {
   EAGLE_ASSERT(queue);
   EagleEvent ev;
   do {
      ev = queue->WaitForEvent(MAIN_THREAD);
      if (ev.type == EAGLE_EVENT_KEY_DOWN) {
         return ev.keyboard.keycode;
      }
   } while (1);
   return 0;
}



bool kb_press(int key) {
   return (keystates[key] & PRESS);
}
bool kb_held(int key) {
   return (keystates[key] & HELD);
}
bool kb_release(int key) {
   return (keystates[key] & RELEASE);
}
bool kb_open(int key) {
   return (keystates[key] & OPEN);
}
bool kb_dblclick(int key) {
   return (keystates[key] & DBLCLICK);
}

bool ms_press(int btn) {
   return (mouse_press & (1 << (btn - 1)));
}
bool ms_held(int btn) {
   return (mouse_held & (1 << (btn - 1)));
}
bool ms_release(int btn) {
   return (mouse_release & (1 << (btn - 1)));
}
bool ms_open(int btn) {
   return (mouse_open & (1 << (btn - 1)));
}
bool ms_dblclick(int btn) {
   return (mouse_dblclick & (1 << (btn - 1)));
}

bool js_press(int joy_num , int btn) {
   return (joysticks[joy_num].buttonstates[btn] & PRESS);
}
bool js_held(int joy_num , int btn) {
   return (joysticks[joy_num].buttonstates[btn] & HELD);
}
bool js_release(int joy_num , int btn) {
   return (joysticks[joy_num].buttonstates[btn] & RELEASE);
}
bool js_open(int joy_num , int btn) {
   return (joysticks[joy_num].buttonstates[btn] & OPEN);
}
bool js_dblclick(int joy_num , int btn) {
   return (joysticks[joy_num].buttonstates[btn] & DBLCLICK);
}



bool js1_press(int btn) {
   return js_press(0 , btn);
}
bool js1_held(int btn) {
   return js_held(0 , btn);
}
bool js1_release(int btn) {
   return js_release(0 , btn);
}
bool js1_open(int btn) {
   return js_open(0 , btn);
}
bool js1_dblclick(int btn) {
   return js_dblclick(0 , btn);
}


bool js2_press(int btn) {
   return js_press(1 , btn);
}
bool js2_held(int btn) {
   return js_held(1 , btn);
}
bool js2_release(int btn) {
   return js_release(1 , btn);
}
bool js2_open(int btn) {
   return js_open(1 , btn);
}
bool js2_dblclick(int btn) {
   return js_dblclick(1 , btn);
}


bool js3_press(int btn) {
   return js_press(2 , btn);
}
bool js3_held(int btn) {
   return js_held(2 , btn);
}
bool js3_release(int btn) {
   return js_release(2 , btn);
}
bool js3_open(int btn) {
   return js_open(2 , btn);
}
bool js3_dblclick(int btn) {
   return js_dblclick(2 , btn);
}


bool js4_press(int btn) {
   return js_press(3 , btn);
}
bool js4_held(int btn) {
   return js_held(3 , btn);
}
bool js4_release(int btn) {
   return js_release(3 , btn);
}
bool js4_open(int btn) {
   return js_open(3 , btn);
}
bool js4_dblclick(int btn) {
   return js_dblclick(3 , btn);
}


bool js5_press(int btn) {
   return js_press(4 , btn);
}
bool js5_held(int btn) {
   return js_held(4 , btn);
}
bool js5_release(int btn) {
   return js_release(4 , btn);
}
bool js5_open(int btn) {
   return js_open(4 , btn);
}
bool js5_dblclick(int btn) {
   return js_dblclick(4 , btn);
}


bool js6_press(int btn) {
   return js_press(5 , btn);
}
bool js6_held(int btn) {
   return js_held(5 , btn);
}
bool js6_release(int btn) {
   return js_release(5 , btn);
}
bool js6_open(int btn) {
   return js_open(5 , btn);
}
bool js6_dblclick(int btn) {
   return js_dblclick(5 , btn);
}


bool js7_press(int btn) {
   return js_press(6 , btn);
}
bool js7_held(int btn) {
   return js_held(6 , btn);
}
bool js7_release(int btn) {
   return js_release(6 , btn);
}
bool js7_open(int btn) {
   return js_open(6 , btn);
}
bool js7_dblclick(int btn) {
   return js_dblclick(6 , btn);
}


bool js8_press(int btn) {
   return js_press(7 , btn);
}
bool js8_held(int btn) {
   return js_held(7 , btn);
}
bool js8_release(int btn) {
   return js_release(7 , btn);
}
bool js8_open(int btn) {
   return js_open(7 , btn);
}
bool js8_dblclick(int btn) {
   return js_dblclick(7 , btn);
}



INPUT_HANDLER input_handlers[NUM_INPUT_SRCS][NUM_INPUT_STATES] = {
   { kb_press  , kb_release  , kb_held  , kb_open  , kb_dblclick } ,
   { ms_press  , ms_release  , ms_held  , ms_open  , ms_dblclick } ,
   { js1_press , js1_release , js1_held , js1_open , js1_dblclick } ,
   { js2_press , js2_release , js2_held , js2_open , js2_dblclick } ,
   { js3_press , js3_release , js3_held , js3_open , js3_dblclick } ,
   { js4_press , js4_release , js4_held , js4_open , js4_dblclick } ,
   { js5_press , js5_release , js5_held , js5_open , js5_dblclick } ,
   { js6_press , js6_release , js6_held , js6_open , js6_dblclick } ,
   { js7_press , js7_release , js7_held , js7_open , js7_dblclick } ,
   { js8_press , js8_release , js8_held , js8_open , js8_dblclick }
};

const char* input_func_text[NUM_INPUT_SRCS][NUM_INPUT_STATES] = {
   { "kb_press" , "kb_release" , "kb_held" , "kb_open" , "kb_dblclick" } ,
   { "ms_press" , "ms_release" , "ms_held" , "ms_open" , "ms_dblclick" } ,
   { "js1_press" , "js1_release" , "js1_held" , "js1_open" , "js1_dblclick" } ,
   { "js2_press" , "js2_release" , "js2_held" , "js2_open" , "js2_dblclick" } ,
   { "js3_press" , "js3_release" , "js3_held" , "js3_open" , "js3_dblclick" } ,
   { "js4_press" , "js4_release" , "js4_held" , "js4_open" , "js4_dblclick" } ,
   { "js5_press" , "js5_release" , "js5_held" , "js5_open" , "js5_dblclick" } ,
   { "js6_press" , "js6_release" , "js6_held" , "js6_open" , "js6_dblclick" } ,
   { "js7_press" , "js7_release" , "js7_held" , "js7_open" , "js7_dblclick" } ,
   { "js8_press" , "js8_release" , "js8_held" , "js8_open" , "js8_dblclick" }
};



/// ----------------------     EagleInputHandler     --------------------------



EagleInputHandler::EagleInputHandler(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      EagleEventSource(),
      EagleEventListener()
{}



void EagleInputHandler::InitializeInput() {
   InitializeKeyboardInput();
   InitializeJoystickInput();
   InitializeMouseInput();
   InitializeTouchInput();
}



void EagleInputHandler::InitializeKeyboardInput() {
   // reserved to change at any time
   ClearKeyState();
   
   // This will always be called
   PrivateInitializeKeyboardInput();
}



void EagleInputHandler::InitializeJoystickInput() {
   // reserved to change at any time
   
   // This will always be called
   PrivateInitializeJoystickInput();
}



void EagleInputHandler::InitializeMouseInput() {
   // reserved to change at any time
   
   // This will always be called
   PrivateInitializeMouseInput();
}



void EagleInputHandler::InitializeTouchInput() {
   // reserved to change at any time
   
   // This will always be called
   PrivateInitializeTouchInput();
}



void EagleInputHandler::ClearKeyState() {
   for (int i = 0 ; i < EAGLE_KEY_STATE_EXTENDED_MAX ; ++i) {
      keystates[i] = OPEN;
   }
   for (int i = 0 ; i < EAGLE_KEY_MAX ; ++i) {
      keydown[i] = 0;
      key_held_duration[i] = 0.0f;
   }
}



void EagleInputHandler::HandleInputEvent(EagleEvent ev) {
   
   if (ev.type == EAGLE_EVENT_NONE) {return;}
   
///   EagleGraphicsContext* old_display = last_display_read;
   mouse_dx = 0;
   mouse_dy = 0;
   mouse_dw = 0;
   mouse_dz = 0;
///   last_display_read = 0;
   

   for (int i = 0 ; i < EAGLE_KEY_STATE_EXTENDED_MAX ; ++i) {
      if (keystates[i] & (PRESS | DBLCLICK)) {keystates[i] = HELD;}
      else if (keystates[i] == RELEASE) {keystates[i] = OPEN;}
   }
   for (int i = 0 ; i < MS_MAX_NUM_BUTTONS ; ++i) {
      int flag = 1 << i;
      if (mouse_press & flag) {
         mouse_press &= ~flag;
         mouse_held |= flag;
      }
      else if (mouse_dblclick & flag) {
         mouse_dblclick &= ~flag;
         mouse_held |= flag;
      }
      else if (mouse_release & (flag)) {
         mouse_release &= ~flag;
         mouse_open |= flag;
      }
   }
   for (int i = 0 ; i < num_joysticks ; ++i) {
      EagleJoystickData& joy = joysticks[i];
      if (joy.pluggedin) {
         for (int b = 0 ; b < joy.num_buttons ; ++b) {
            if (joy.buttonstates[b] & (PRESS | DBLCLICK)) {joy.buttonstates[b] = HELD;}
            else if (joy.buttonstates[b] == RELEASE) {joy.buttonstates[b] = OPEN;}
         }
      }
   }
/*
x    * EAGLE_EVENT_JOYSTICK_AXIS
x    * EAGLE_EVENT_JOYSTICK_BUTTON_DOWN
x    * EAGLE_EVENT_JOYSTICK_BUTTON_UP
x    * EAGLE_EVENT_JOYSTICK_CONFIGURATION
x    * EAGLE_EVENT_KEY_DOWN
x    * EAGLE_EVENT_KEY_UP
-    * EAGLE_EVENT_KEY_CHAR
x    * EAGLE_EVENT_MOUSE_AXES
x    * EAGLE_EVENT_MOUSE_BUTTON_DOWN
x    * EAGLE_EVENT_MOUSE_BUTTON_UP
x    * EAGLE_EVENT_MOUSE_WARPED
x    * EAGLE_EVENT_MOUSE_ENTER_DISPLAY
x    * EAGLE_EVENT_MOUSE_LEAVE_DISPLAY
x    * EAGLE_EVENT_TIMER
 * EAGLE_EVENT_DISPLAY_EXPOSE
 * EAGLE_EVENT_DISPLAY_RESIZE
 * EAGLE_EVENT_DISPLAY_CLOSE
 * EAGLE_EVENT_DISPLAY_LOST
 * EAGLE_EVENT_DISPLAY_FOUND
 * EAGLE_EVENT_DISPLAY_SWITCH_OUT
 * EAGLE_EVENT_DISPLAY_SWITCH_IN
 * EAGLE_EVENT_DISPLAY_ORIENTATION
*/
   if (ev.type == EAGLE_EVENT_TIMER) {
      if (input_timer && (ev.timer.raw_source == input_timer->Source())) {
// since_last_jspress
// since_last_keypress
// since_last_mspress
         float dt = input_timer->SecondsPerTick();
         for (int i = 0 ; i < EAGLE_KEY_MAX ; ++i) {
            if (keystates[i] == HELD) {
               key_held_duration[i] += dt;
            }
            since_last_keypress[i] += dt;
         }
         for (int i = EAGLE_KEY_MAX ; i < EAGLE_KEY_STATE_EXTENDED_MAX ; ++i) {
            if (keystates[i] == HELD) {
               key_held_duration[i] += dt;
            }
         }
         for (int i = 0 ; i < MS_MAX_NUM_BUTTONS ; ++i) {
            if (mouse_held & (1 << i)) {
               msbtn_held_duration[i] += dt;
            }
            since_last_mspress[i] += dt;
         }
         for (int i = 0 ; i < num_joysticks ; ++i) {
            EagleJoystickData& joy = joysticks[i];
            if (joy.pluggedin) {
               for (int b = 0 ; b < joy.num_buttons ; ++b) {
                  if (joy.buttonstates[b] == HELD) {
                     joy.button_held_duration[b] += dt;
                  }
                  joy.since_last_jspress[b] += dt;
               }
            }
         }
      }
   }
   else if (ev.type == EAGLE_EVENT_MOUSE_AXES) {
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_w = ev.mouse.w;
      mouse_z = ev.mouse.z;
      mouse_dx = ev.mouse.dx;
      mouse_dy = ev.mouse.dy;
      mouse_dw = ev.mouse.dw;
      mouse_dz = ev.mouse.dz;
   }
   else if (ev.type == EAGLE_EVENT_JOYSTICK_AXIS) {
/*
      int index = -1;
      for (int i = 0 ; i < num_joysticks ; ++i) {
         if (ev.joystick.id == joysticks[i].joystick) {index = i;}
      }
      if (index != -1) {
         joysticks[index].axes[ev.joystick.stick][ev.joystick.axis] = ev.joystick.pos;
      }
*/
      joysticks[ev.joystick.nid].axes[ev.joystick.stick][ev.joystick.axis] = ev.joystick.pos;
   }
   else if (ev.type == EAGLE_EVENT_KEY_DOWN) {
      if (since_last_keypress[ev.keyboard.keycode] < double_click_duration) {
         keystates[ev.keyboard.keycode] = DBLCLICK;
      }
      else {
         keystates[ev.keyboard.keycode] = PRESS;
      }
      since_last_keypress[ev.keyboard.keycode] = 0.0f;
      keydown[ev.keyboard.keycode] = true;
      UpdateModKeys();
   }
   else if (ev.type == EAGLE_EVENT_KEY_UP) {
      keystates[ev.keyboard.keycode] = RELEASE;
      keydown[ev.keyboard.keycode] = false;
      key_held_duration[ev.keyboard.keycode] = 0.0f;
      UpdateModKeys();
   }
   else if (ev.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_w = ev.mouse.w;
      mouse_z = ev.mouse.z;
      int flag = 1 << (ev.mouse.button - 1);
      mouse_press |= flag;
      if (since_last_mspress[ev.mouse.button] < double_click_duration) {
         mouse_dblclick |= flag;
      }
      since_last_mspress[ev.mouse.button] = 0.0f;
      mouse_open &= ~flag;
      mouse_release &= ~flag;
      mouse_down |= flag;
///      last_display_read = ev.mouse.display;
   }
   else if (ev.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_w = ev.mouse.w;
      mouse_z = ev.mouse.z;
      int flag = 1 << (ev.mouse.button - 1);
      mouse_release |= flag;
      mouse_press &= ~flag;
      mouse_held &= ~flag;
      mouse_down &= ~flag;
      mouse_dblclick &= ~flag;
///      last_display_read = ev.mouse.display;
   }
   else if (ev.type == EAGLE_EVENT_JOYSTICK_BUTTON_DOWN) {
/**
      int index = -1;
      for (int i = 0 ; i < num_joysticks ; ++i) {
         /// joysticks[i] = EagleJoystickData
         /// joysticks[i].joystick = void* to joystick source
         /// ev.joystick.id = EagleJoystickData*
         if (joysticks[i].joystick == ev.joystick.id) {
            index = i;
         }
      }
      if (index != -1) {
         if (joysticks[index].since_last_jspress[ev.joystick.button] < double_click_duration) {
            joysticks[index].buttonstates[ev.joystick.button] = DBLCLICK;
         }
         else {
            joysticks[index].buttonstates[ev.joystick.button] = PRESS;
         }
         joysticks[index].since_last_jspress[ev.joystick.button] = 0.0f;
      }
*/
      int index = ev.joystick.nid;
      if (joysticks[index].since_last_jspress[ev.joystick.button] < double_click_duration) {
         joysticks[index].buttonstates[ev.joystick.button] = DBLCLICK;
      }
      else {
         joysticks[index].buttonstates[ev.joystick.button] = PRESS;
      }
      joysticks[index].since_last_jspress[ev.joystick.button] = 0.0f;
   }
   else if (ev.type == EAGLE_EVENT_JOYSTICK_BUTTON_UP) {
/*
      int index = -1;
      for (int i = 0 ; i < num_joysticks ; ++i) {
         if (joysticks[i].joystick == ev.joystick.id) {
            index = i;
         }
      }
      if (index != -1) {
         joysticks[index].buttonstates[ev.joystick.button] = RELEASE;
      }
*/
      joysticks[ev.joystick.nid].buttonstates[ev.joystick.button] = RELEASE;
   }
   else if (ev.type == EAGLE_EVENT_JOYSTICK_CONFIGURATION) {
/*
      if (al_reconfigure_joysticks()) {
         InitializeJoysticks();
      }
*/

      InitializeJoystickInput();

   }
   else if (ev.type == EAGLE_EVENT_MOUSE_WARPED) {
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_w = ev.mouse.w;
      mouse_z = ev.mouse.z;
      mouse_dx = ev.mouse.dx;
      mouse_dy = ev.mouse.dy;
      mouse_dw = ev.mouse.dw;
      mouse_dz = ev.mouse.dz;
///      last_display_read = ev.mouse.display;
   }
   else if (ev.type == EAGLE_EVENT_MOUSE_ENTER_DISPLAY) {
      mouse_on = true;
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_z = ev.mouse.z;
      mouse_w = ev.mouse.w;
///      last_display_read = old_display;
   }
   else if (ev.type == EAGLE_EVENT_MOUSE_LEAVE_DISPLAY) {
      mouse_on = false;
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_z = ev.mouse.z;
      mouse_w = ev.mouse.w;
///      last_display_read = old_display;
   }
}



void EagleInputHandler::RecordInputPress(EagleEventHandler* queue , Input* input) {
   EAGLE_ASSERT(queue);
   EAGLE_ASSERT(input);
//   al_flush_event_queue(queue);
   while (1) {
      EagleEvent ev;
      ev = queue->WaitForEvent(MAIN_THREAD);
      HandleInputEvent(ev);
      Input i;
      if (AnyInputPressed(&i)) {
         *input = i;
         break;
      }
   }
}



void EagleInputHandler::RecordInputGroup(EagleEventHandler* queue , InputGroup* input_group) {
   EAGLE_ASSERT(queue);
   EAGLE_ASSERT(input_group);
   
   Input press;
   Input held;
   
   RecordInputPress(queue , &press);
   if (ModifierHeld(&held)) {
      *input_group = press && held;
      return;
   }
   *input_group = InputGroup(press);
}


