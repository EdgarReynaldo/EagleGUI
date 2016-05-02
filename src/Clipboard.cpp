


#include "Eagle/Clipboard.hpp"
///#include "Platform.hpp"
#include "Eagle/GraphicsContext.hpp"




void EagleClipboard::CopyToClipboard(string str) {
   clipboard_string = str;
}



string EagleClipboard::GetClipboardString() {
   return clipboard_string;
}



void EagleClipboard::SetDisplayWindow(EagleGraphicsContext* win) {
   window = win;
}








