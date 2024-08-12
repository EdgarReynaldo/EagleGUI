
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
 * @file InputAssignment.cpp
 * @brief Implementation of input assignment class mapping.
 *
 */

 

 
 
#include "Eagle/InputAssignment.hpp"




/// ---------------------------------- InputAssignments class ------------------------------------------



void InputAssignment::clear() {
   task_input_map.clear();
}



InputGroup& InputAssignment::operator[](const std::string& str) {
   TIMIT timit = task_input_map.find(str);
   if (timit == task_input_map.end()) {
//      throw EagleException(StringPrintF("Failed to find %s in input map." , str.c_str()));
   }
   return task_input_map[str];
}



bool InputAssignment::Activated(const std::string& str) {/** TODO : Is there a better function name than Activated? */
   TIMIT timit = task_input_map.find(str);
   if (timit != task_input_map.end()) {
      return ((bool)(timit->second));
   }
   return false;// InputGroup not found in map
}





