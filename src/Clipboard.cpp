


#include "Eagle/Clipboard.hpp"
#include "Eagle/GraphicsContext.hpp"




EagleClipboard::EagleClipboard(std::string objclass , std::string objname) : 
      EagleObject(objclass , objname),
      window(0),
      clipboard_string("")
{}



void EagleClipboard::CopyToClipboard(string str) {
   clipboard_string = str;
}



string EagleClipboard::GetClipboardString() {
   return clipboard_string;
}



void EagleClipboard::SetDisplayWindow(EagleGraphicsContext* win) {
   window = win;
}








