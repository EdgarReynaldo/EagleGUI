
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Clipboard.hpp"
#include "Eagle/GraphicsContext.hpp"




EagleClipboard::EagleClipboard(std::string objclass , std::string objname) : 
      EagleObject(objclass , objname),
      window(0),
      clipboard_string("")
{}



void EagleClipboard::CopyToClipboard(std::string str) {
   clipboard_string = str;
}



std::string EagleClipboard::GetClipboardString() {
   return clipboard_string;
}



void EagleClipboard::SetDisplayWindow(EagleGraphicsContext* win) {
   window = win;
}








