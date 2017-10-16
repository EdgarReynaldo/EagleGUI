



#ifndef Clipboard_HPP
#define Clipboard_HPP


#include "Eagle/Object.hpp"



#include <string>
using std::string;



class EagleGraphicsContext;




///class ClipboardData



class EagleClipboard : public EagleObject {

protected :
   EagleGraphicsContext* window;

   string clipboard_string;

   
public :
///   Clipboard();
   EagleClipboard(std::string objclass = "EagleClipboard" , std::string objname = "Nemo");

   virtual ~EagleClipboard() {}
   
   virtual void CopyToClipboard(string str);
   virtual string GetClipboardString();
   
   void SetDisplayWindow(EagleGraphicsContext* win);
};


#endif // Clipboard_HPP

