



#ifndef Clipboard_HPP
#define Clipboard_HPP

#include <string>
using std::string;



class EagleGraphicsContext;


class EagleClipboard {

protected :
   EagleGraphicsContext* window;

   string clipboard_string;

   
public :
///   Clipboard();
   EagleClipboard() : 
         window(0),
         clipboard_string("")
   {}

   virtual ~EagleClipboard() {}
   
   virtual void CopyToClipboard(string str);
   virtual string GetClipboardString();
   
   void SetDisplayWindow(EagleGraphicsContext* win);
};


#endif // Clipboard_HPP

