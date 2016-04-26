



#ifndef Text_HPP
#define Text_HPP

#include "Eagle/Gui/Alignment.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/StringWork.hpp"

#include <vector>
#include <string>

void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight);



class DumbText : public WidgetBase {

private :
   HALIGNMENT halign;
   VALIGNMENT valign;
   std::string text;
   EagleFont* text_font;
   int hpadding;
   int vpadding;
   int linespacing;
   
protected :
   void DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c);

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   
public :
///   DumbText();
   DumbText() :
         WidgetBase(StringPrintF("Text widget at %p" , this)),
         halign(HALIGN_CENTER),
         valign(VALIGN_CENTER),
         text(""),
         text_font(0),
         hpadding(0),
         vpadding(0),
         linespacing(0)
   {}
   
   void SetTextParameters(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing);
   void SetTextString(std::string str , EagleFont* font = 0);
   
};

#endif // Text_HPP
