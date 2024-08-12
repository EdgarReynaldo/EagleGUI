
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
 * @file InputAssignment.hpp
 * @brief This file contains the interface for the InputAssignment class, a map between input names and input groups.
 * 
 */




#ifndef EagleInputAssignment_HPP
#define EagleInputAssignment_HPP



#include <map>
#include <string>

#include "Eagle/InputGroup.hpp"


/**! @class InputAssignment
 *   @brief A simple but useful class for working with input assignments. Input names are matched to InputGroup objects.
 *   
 *   
 *    Usage example
 *    
 @code 
      void AssignPlayerDefaults(InputAssignment& player) {
         player.clear();
         player["Punch"]     = input_key_press(EAGLE_KEY_ONLY_SHIFT);
         player["Kick"]      = input_key_press(EAGLE_KEY_ONLY_ALT);
         player["Jump"]      = input_key_press(EAGLE_KEY_UP);
         player["Crouch"]    = input_key_held(EAGLE_KEY_DOWN);
         player["MoveLeft"]  = input_key_held(EAGLE_KEY_LEFT);
         player["MoveRight"] = input_key_held(EAGLE_KEY_RIGHT);
         player["Quit"]      = input_key_held(EAGLE_KEY_ANY_CTRL) && input_key_press(EAGLE_KEY_ESCAPE);
      }
      InputAssignment Player;
      AssignPlayerDefaults(Player);
  
      if (Player.Activated("Punch")) {...}
      if (player["Punch"]) {...}  /// This should work as well, [] returns an InputGroup& and if () should evaluate it
 @endcode
 */

class InputAssignment {
private :
   typedef std::map<std::string , InputGroup> TASKINPUTMAP;
   typedef TASKINPUTMAP::iterator TIMIT;

   TASKINPUTMAP task_input_map;

public :
   InputAssignment() : task_input_map() {}///< Default empty constructor

   void clear();///< Clears the input map

   InputGroup& operator[](const std::string& str);///< Retrieves a reference to the InputGroup associated with @param str

   bool Activated(const std::string& str);///< True if this control was activated
};




#endif // EagleInputAssignment_HPP
