




#ifndef Allegro5Clipboard_HPP
#define Allegro5Clipboard_HPP


#include "Eagle/Clipboard.hpp"
#include <string>
using std::string;



class Allegro5Clipboard : public EagleClipboard {

protected :
   
public :
   Allegro5Clipboard(std::string objname = "Nemo");
   
   virtual void CopyToClipboard(string str);
   virtual string GetClipboardString();   
};


#endif // Allegro5Clipboard_HPP

