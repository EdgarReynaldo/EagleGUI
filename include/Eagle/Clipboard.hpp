
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Clipboard.hpp
 * @brief The interface for all clipboard functions in Eagle
 * 
 * TODO : Under development.
 * 
 */

#ifndef Clipboard_HPP
#define Clipboard_HPP

#warning TODO : Support more than local strings... Use my clipboard library instead.



#include "Eagle/Object.hpp"

#include <string>



class EagleGraphicsContext;



class EagleClipboard : public EagleObject {

protected :
   EagleGraphicsContext* window;

   std::string clipboard_string;

   
public :
///   Clipboard();
   EagleClipboard(std::string objclass = "EagleClipboard" , std::string objname = "Nemo");

   virtual ~EagleClipboard() {}
   
   virtual void CopyToClipboard(std::string str);
   virtual std::string GetClipboardString();
   
   void SetDisplayWindow(EagleGraphicsContext* win);
};




#endif // Clipboard_HPP



