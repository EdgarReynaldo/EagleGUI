
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



#include "Eagle/Exception.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/System.hpp"

#include <sstream>

using std::ostringstream;
using std::endl;
using std::ostream;
using std::string;



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
}

VALUE_TO_NAME_FUNC value_to_name_func[NUM_INPUT_SRCS] = {
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



void EagleInputHandler::RegisterInputs(EagleEventHandler* queue) {
	RegisterKeyboardInput(queue);
	RegisterMouseInput(queue);
	RegisterJoystickInput(queue);
	RegisterTouchInput(queue);
}



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

EagleGraphicsContext* last_display_read = 0;





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



void HandleInputEvent(EagleEvent ev) {
   
   if (ev.type == EAGLE_EVENT_NONE) {return;}
   
   EagleGraphicsContext* old_display = last_display_read;
   mouse_dx = 0;
   mouse_dy = 0;
   mouse_dw = 0;
   mouse_dz = 0;
   last_display_read = 0;
   

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
      last_display_read = ev.mouse.display;
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
      last_display_read = ev.mouse.display;
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
      last_display_read = ev.mouse.display;
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
      EAGLE_ASSERT(eagle_system);
      EagleInputHandler* input = eagle_system->GetInputHandler();
      if (input) {
         input->InitializeJoystickInput();
      }
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
      last_display_read = ev.mouse.display;
   }
   else if (ev.type == EAGLE_EVENT_MOUSE_ENTER_DISPLAY) {
      mouse_on = true;
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_z = ev.mouse.z;
      mouse_w = ev.mouse.w;
      last_display_read = old_display;
   }
   else if (ev.type == EAGLE_EVENT_MOUSE_LEAVE_DISPLAY) {
      mouse_on = false;
      mouse_x = ev.mouse.x;
      mouse_y = ev.mouse.y;
      mouse_z = ev.mouse.z;
      mouse_w = ev.mouse.w;
      last_display_read = old_display;
   }
}




int ReadKey(EagleEventHandler* queue) {
   EAGLE_ASSERT(queue);
   EagleEvent ev;
   do {
      ev = queue->WaitForEvent();
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



Input::Input() :
      src(KB),
      state(PRESS),
      value(0),
      input_handler_func(0)
{
   AssignTo(KB , PRESS , EAGLE_KEY_NONE);
}



Input::Input(INPUTSRC source , INPUTSTATE inputstate , int input_value) :
      src(KB),
      state(PRESS),
      value(0),
      input_handler_func(0)
{
   AssignTo(source , inputstate , input_value);
}



void Input::AssignTo(INPUTSRC source , INPUTSTATE inputstate , int input_value) {
   src = source;
   state = inputstate;
   value = input_value;
   
   int stateindex = -1;
   int tempstate = state;
   while (tempstate) {
      tempstate >>= 1;
      stateindex += 1;
   }
   
   input_handler_func = input_handlers[src][stateindex];
}



Input::operator bool() const {
   return input_handler_func(value);
}



Input::operator int() const {
   bool True = (bool)(*this);
   if (True) {
      return 1;
   }
   return 0;
}
   


bool Input::operator==(const Input& i) {
   return ((value == i.value) && (input_handler_func == i.input_handler_func));
}



bool Input::operator!=(const Input& i) {
   return !(*this == i);
}


std::ostream& Input::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "Input <src = \"" << input_source_str[src] << "\" type = \"" << input_state_str[state];
   os << "\" value = \"" << value_to_name_func[src](value) << "\" >";
   return os;
}

std::ostream& operator<<(std::ostream& os , const Input& input) {
   /** TODO : If the scancode enumeration is extended for things like KEY_CTRL_EITHER,
               then scancode_to_name will have to be extended with a wrapper function. */
   return input.DescribeTo(os);
}


std::ostream& Input::Shortname(std::ostream& os) const {
   int stateindex = -1;
   int tempstate = state;
   while (tempstate) {
      tempstate >>= 1;
      stateindex += 1;
   }
   os << input_func_text[src][stateindex] << "(" << value_to_name_func[src](value) << ")";
   return os;
}



/** #################            InputGroup class methods               ################# */



void InputGroup::SetNodeLevel(int level) const {
   node_level = level;
   const int num_nodes = nodes.size();
   for (int i = 0 ; i < num_nodes ; ++i) {nodes[i].SetNodeLevel(level + 1);}
}



InputGroup::InputGroup() : all_apply(true) , is(true) , inputs() , nodes() , node_level(0) {}



InputGroup::InputGroup(const Input& i) : all_apply(true) , is(true) , inputs() , nodes() , node_level(0) {
   inputs.push_back(i);
}



InputGroup::InputGroup(const InputGroup& ig) :
            all_apply(ig.all_apply) , is(ig.is) , inputs() , nodes() , node_level(ig.node_level) {
/** TODO : where is this warning coming from? -
/mingw/include/c++/3.4.5/bits/stl_vector.h: In member function `std::vector<_Tp, _Alloc>& std::vector<_Tp, _Alloc>::operator=(const std::vector<_Tp, _Alloc>&) [with _Tp = InputGroup, _Alloc = std::allocator<InputGroup>]':
/mingw/include/c++/3.4.5/bits/stl_vector.h:715: warning: '__result' might be used uninitialized in this function
/mingw/include/c++/3.4.5/bits/stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in this function
/mingw/include/c++/3.4.5/bits/stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in this function
*/
   inputs = ig.inputs;
   nodes = ig.nodes;
}



InputGroup::InputGroup(bool use_and , bool use_is , const Input& i) :
            all_apply(use_and) , is(use_is) , inputs() , nodes() , node_level(0) {
   inputs.push_back(i);
}



InputGroup::InputGroup(bool use_and , bool use_is , const InputGroup& ig) :
            all_apply(use_and) , is(use_is) , inputs() , nodes() , node_level(0) {
   push_back(ig);
   /** Previous version, doesn't allow for consolidation 
   ig.SetNodeLevel(1);
   nodes.push_back(ig);
   */
}



void InputGroup::clear() {
   inputs.clear();
   nodes.clear();
}



InputGroup& InputGroup::push_back(const Input& i) {
   inputs.push_back(i);
   return *this;
}



InputGroup& InputGroup::push_back(const InputGroup& ig) {

   /** This function is Re-entrant */
   /** If the InputGroup to add is the same type as the current one,
         add their inputs to this InputGroup's input list and try to 
         add their nodes to this InputGroup's node list by re entering
         the function.
         If the InputGroup is not the same type then it needs to be added to
         the nodes list.
   */

   if (SameTypeAs(ig)) {
      const int num_new = ig.inputs.size();
      for (int i = 0 ; i < num_new ; ++i) {
         push_back(ig.inputs[i]);
      }
      const int num_new_groups = ig.nodes.size();
      for (int i = 0 ; i < num_new_groups ; ++i) {
         push_back(ig.nodes[i]);
      }
   } else {
      /* Original behavior */
      ig.SetNodeLevel(node_level + 1);
      nodes.push_back(ig);
   }

   return *this;
}


/** TODO : Working here - the behavior of && and || may be able to consolidate more inputs */

InputGroup InputGroup::operator&&(const InputGroup& ig) const {
   InputGroup clump(true , true , ig);
   clump.push_back(*this);
   return clump;
}



InputGroup InputGroup::operator||(const InputGroup& ig) const {
   InputGroup clump(false , true , ig);
   clump.push_back(*this);
   return clump;
}



InputGroup InputGroup::operator!() const {
   /** Return an InputGroup with the value of 'is' negated? Or a boolean value?      */
   /** Input group ==> " !(Input(A) || Input(B)) " ==> " !InputGroup(A || B) "       */
   /** As in, we don't want to leave a bool value in a grouping chain.               */
   /** This should still work with evaluations though, as "if (!InputGroup)" should  */
   /** end up as !InputGroup::operator bool.                                          */

   /* TODO : Can't this just return a reference to itself with the value of 'is' negated? */
   /*      - No, not for constant objects, so a new copy needs to be created and returned
   is = !is;
   return (*this);
   */
   InputGroup ig(*this);
   ig.is = !(ig.is);
   return ig;
}



InputGroup::operator bool() const {
   bool ret;
   const int num_inputs = inputs.size();
   const int num_nodes = nodes.size();
   if ((num_inputs + num_nodes) == 0) {
      return false;
      /// throw InputGroupError("Trying to evaluate an empty input group!");
   }
   if (all_apply) {
      ret = true;
      /** AND the inputs and nodes */
      for (int i = 0 ; i < num_inputs ; ++i) {
         /** If any are false, the whole AND group is false */
         if (!(bool)inputs[i]) {return IS(false);}
      }
      for (int i = 0 ; i < num_nodes ; ++i) {
         if (!(bool)nodes[i]) {return IS(false);}
      }
      
   } else {
      ret = false;
      /** OR the inputs and nodes */
      for (int i = 0 ; i < num_inputs ; ++i) {
         /** If any are true, the whole OR group is true */
         if (inputs[i]) {return IS(true);}
      }
      for (int i = 0 ; i < num_nodes ; ++i) {
         if (nodes[i]) {return IS(true);}
      }
   }
   return IS(ret);
}



bool InputGroup::PointerInput() const {
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if ((inputs[i].Source() != KB) && (inputs[i].State() == PRESS)) {return true;}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].PointerInput()) {return true;}
   }
   return false;
}



bool InputGroup::HasClickInput() const {
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if (inputs[i].State() == PRESS) {return true;}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].HasClickInput()) {return true;}
   }
   return false;
}



Input InputGroup::FindClickInput() const {
   const Input false_return = input_key_press(EAGLE_KEY_NONE);
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if (inputs[i].State() == PRESS) {return inputs[i];}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].FindClickInput() != false_return) {return nodes[i].FindClickInput();}
   }
   return false_return;
}



std::ostream& InputGroup::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "InputGroup [Logical ";
   os << (all_apply?"AND":"OR") << " , " << (is?"IS":"NOT") << "]" << endl;

   os << indent << "{" << endl;
   ++indent;
   const int num_inputs = inputs.size();
   const int num_nodes  = nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      os << indent << inputs[i] << endl;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      nodes[i].DescribeTo(os,indent);
   }
   --indent;
   os << indent << "}" << endl;
   return os;
}



std::ostream& operator<<(std::ostream& os , const InputGroup& ig) {
   return ig.DescribeTo(os);
/**
   Indenter indent(ig.node_level,3);
   Indenter indent_input(ig.node_level + 1 , 3);
   os << indent << "InputGroup [Logical ";
   os << (ig.all_apply?"AND":"OR") << " , " << (ig.is?"IS":"NOT") << "]" << endl;
   os << indent << "{" << endl;
   const int num_inputs = ig.inputs.size();
   const int num_nodes  = ig.nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      os << indent_input << ig.inputs[i] << endl;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      os << ig.nodes[i] << endl;
   }
   os << indent << "}" << endl;
   return os;
*/
}



ostream& InputGroup::ShowLogic(ostream& os) const {
   const int num_inputs = inputs.size();
   const int num_nodes = nodes.size();
///   const int num_items = inputs.size() + nodes.size();
///   int item_num = 0;
   
   if (!is) {os << "!";}
   os << "(";
   for (int i = 0 ; i < num_inputs ; ++i) {
      inputs[i].Shortname(os);
      if ((i + 1) != num_inputs) {
         if (all_apply) {
            os << " && ";
         } else {
            os << " || ";
         }
      }
///      ++item_num;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      if (( num_inputs > 0) || (i > 0)) {
         if (all_apply) {
            os << " && ";
         } else {
            os << " || ";
         }
      }
      nodes[i].ShowLogic(os);
   }
   os << ")";
   return os;
}



void InputGroup::WriteLines(std::vector<std::string>* strvec) {
   ostringstream ostr;
   Indenter indent(node_level,3);
   Indenter indent_input(node_level + 1 , 3);
   ostr << indent << "InputGroup [Logical ";
   ostr << (all_apply?"AND":"OR") << " , " << (is?"IS":"NOT") << "]";
   strvec->push_back(ostr.str());
   ostr.str("");
   ostr << indent << "{";
   strvec->push_back(ostr.str());
   ostr.str("");

   const int num_inputs = inputs.size();
   const int num_nodes  = nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      ostr << indent_input << inputs[i];
      strvec->push_back(ostr.str());
      ostr.str("");
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      nodes[i].WriteLines(strvec);
//      ostr << ig.nodes[i] << endl;
   }
   ostr << indent << "}";
   strvec->push_back(ostr.str());
   ostr.str("");
}




/** ######   Global operators for joining Input objects into InputGroup objects   ######## */



InputGroup operator&&(const Input& i1 , const Input& i2) {
   return InputGroup(true , true , i1).push_back(i2);
}



InputGroup operator||(const Input& i1 , const Input& i2) {
   return InputGroup(false , true , i1).push_back(i2);
}



InputGroup operator&&(const Input& i , const InputGroup& ig) {
   InputGroup ignew(ig);
   return (ignew && i);
}



InputGroup operator||(const Input& i , const InputGroup& ig) {
   InputGroup ignew(ig);
   /// TODO : This may be wrong? - fix it?.
   return (ignew || i);
}



InputGroup operator!(const Input& i) {
   return InputGroup(true , false , i);
}



bool AnyInputPressed(Input* store) {
   for (int i = 0 ; i < EAGLE_KEY_MAX ; ++i) {
      if (keystates[i] & PRESS) {
         if (store) {
            store->AssignTo(KB , PRESS , i);
         }
         return true;
      }
   }
   for (int i = 0 ; i < MS_MAX_NUM_BUTTONS ; ++i) {
      if (mouse_press & (1 << i)) {
         if (store) {
            store->AssignTo(MS , PRESS , i + 1);
         }
         return true;
      }
   }
   for (int joy_num = 0 ; joy_num < num_joysticks ; ++joy_num) {
      for (int b = 0 ; b < joysticks[joy_num].num_buttons ; ++b) {
         if (joysticks[joy_num].buttonstates[b] & PRESS) {
            if (store) {
               store->AssignTo((INPUTSRC)(JS1 + joy_num) , PRESS , b);
            }
            return true;
         }
      }
   }
   return false;
}


/// TODO : This function looks funny... I don't think it does what it is supposed to...
bool NonModInputPressed(Input* store) {
   if (!store) {return false;}
   for (int i = 0 ; i < EAGLE_KEY_MAX ; ++i) {
      if ((keystates[i] & PRESS) && ((i < EAGLE_KEY_LSHIFT) || (i > EAGLE_KEY_ALTGR))) {
         store->AssignTo(KB , PRESS , i);
         return true;
      }
   }
   for (int i = 0 ; i < MS_MAX_NUM_BUTTONS ; ++i) {
      if (mouse_press & (1 << i)) {
         store->AssignTo(MS , PRESS , i + 1);
         return true;
      }
   }
   for (int joy_num = 0 ; joy_num < num_joysticks ; ++joy_num) {
      for (int b = 0 ; b < joysticks[joy_num].num_buttons ; ++b) {
         if (joysticks[joy_num].buttonstates[b] & PRESS) {
            store->AssignTo((INPUTSRC)(JS1 + joy_num) , PRESS , b);
            return true;
         }
      }
   }
   return false;
}



bool ModifierHeld(Input* store) {
   for (int i = EAGLE_KEY_ONLY_SHIFT ; i < EAGLE_KEY_STATE_EXTENDED_MAX ; ++i) {
      if (keystates[i] & HELD) {
         if (store) {
            store->AssignTo(KB , HELD , i);
         }
         return true;
      }
   }
   /** TODO : Add in mouse buttons held and joystick buttons or touch buttons held */
   return false;
}



void RecordInputPress(EagleEventHandler* queue , Input* input) {
   EAGLE_ASSERT(queue);
   EAGLE_ASSERT(input);
//   al_flush_event_queue(queue);
   while (1) {
      EagleEvent ev;
      ev = queue->WaitForEvent();
      HandleInputEvent(ev);
      Input i;
      if (AnyInputPressed(&i)) {
         *input = i;
         break;
      }
   }
}



void RecordInputGroup(EagleEventHandler* queue , InputGroup* input_group) {
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





/// ---------------------------------- InputAssignments class ------------------------------------------



void InputAssignment::clear() {
   task_input_map.clear();
}



InputGroup& InputAssignment::operator[](const string& str) {
   return task_input_map[str];
}



bool InputAssignment::Activated(const string& str) {/** TODO : Is there a better function name than Activated? */
   TIMIT timit = task_input_map.find(str);
   if (timit != task_input_map.end()) {
      return ((bool)(timit->second));
   }
   return false;// InputGroup not found in map
}










