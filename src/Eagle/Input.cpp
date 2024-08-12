
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Input.cpp
 * @brief Implementation of Input class functions
 * 
 */

 
#include <iostream>
 
#include "Eagle/Input.hpp"
#include "Eagle/InputHandler.hpp"
 
 
 /// ---------------------------     Input class     ------------------------------



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



bool AnyKeyPressed(Input* store) {
   Input i;
   if (AnyInputPressed(&i)) {
      if (i.Source() == KB) {
         if (store) {*store = i;}
         return true;
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






