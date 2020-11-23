
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5Clipboard.hpp
 * @brief The Allegro 5 implementation for the clipboard
 * 
 * TODO : Replace with my clipboard
 */

#ifndef Allegro5Clipboard_HPP
#define Allegro5Clipboard_HPP



#include "Eagle/Clipboard.hpp"
#include <string>



class Allegro5Clipboard : public EagleClipboard {

protected :
   
public :
   Allegro5Clipboard(std::string objname = "Nemo");///< Default constructor
   
   virtual void CopyToClipboard(std::string str);///< Copy the text string to the clipboard
   virtual std::string GetClipboardString();///< Get the text in the clipboard
};




#endif // Allegro5Clipboard_HPP



