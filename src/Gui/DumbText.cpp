



#include "Eagle/Gui/DumbText.hpp"


#include <iostream>
#include <string>
using namespace std;


#include "Eagle/Platform.hpp"
#include "Eagle/Gui/Text.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"


#include <cstdlib>


void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight) {

   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
///   EAGLE_ASSERT(pnlines);
///   EAGLE_ASSERT(plines);
///   EAGLE_ASSERT(pwidths_vector);
   EAGLE_ASSERT(pmaxwidth);
   EAGLE_ASSERT(ptotalheight);

   /// calculate size of entire text
   if (plines) {*plines = SplitByNewLines(text);}
   if (pnlines) {
      if (plines) {
         *pnlines = (int)plines->size();
      }
   }
   if (*pnlines < 1) {
      return;
   }
   int lineheight = font->Height();
   if (pwidths_vector) {pwidths_vector->clear();}
   *pmaxwidth = 0;
   for (int i = 0 ; i < *pnlines ; ++i) {
      int w = font->Width((*plines)[i].c_str());
      if (pwidths_vector) {pwidths_vector->push_back(w);}
      if (w > *pmaxwidth) {*pmaxwidth = w;}
   }
   *ptotalheight = lineheight*(*pnlines) + line_spacing*(*pnlines-1);

}


void DumbText::RefreshTextPosition(int lineheight) {

   EAGLE_ASSERT(text_font);
   EAGLE_ASSERT(text_font->Valid());
   lines = SplitByNewLines(text);
   nlines = lines.size();
   
///   int lineheight = text_font->Height();
   maxwidth = 0;
   totalheight = lineheight*nlines + linespacing*(nlines-1);

   lineareas.clear();
   
   if (nlines < 1) {return;}

   int y = InnerArea().Y() + vpadding;
   if (valign == VALIGN_CENTER) {
      y = InnerArea().CY() - totalheight/2;
   }
   if (valign == VALIGN_BOTTOM) {
      y = InnerArea().BRY() - vpadding - totalheight;
   }
   texty = y;
   for (int i = 0 ; i < nlines ; ++i) {
      int w = text_font->Width(lines[i].c_str());
      int x = InnerArea().X() + hpadding;
      if (halign == HALIGN_CENTER) {
         x = InnerArea().CX() - w/2;
      }
      if (halign == HALIGN_RIGHT) {
         x = InnerArea().BRX() - hpadding - w;
      }
      if (w > maxwidth) {
         maxwidth = w;
         textx = x;
      }
      lineareas.push_back(Rectangle(x,y,w,lineheight));
      y += lineheight;
   }
   SetBgRedrawFlag();
}



int DumbText::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int DumbText::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void DumbText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DrawText(win , xpos , ypos , WCols()[TXTCOL]);
}



int DumbText::PrivateUpdate(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}


/**
   HALIGNMENT halign;
   VALIGNMENT valign;
   EagleFont* text_font;
   string text;
   std::vector<std::string> lines;
   std::vector<Rectangle> lineareas;
   int nlines;
   int hpadding;
   int vpadding;
   int linespacing;
   int maxwidth;
   int totalheight;
   std::vector<int> widths_vector;

//*/
DumbText::DumbText() :
      WidgetBase(StringPrintF("Dumb Text object at %p" , this)),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      text_font(0),
      text(""),
      lines(),
      lineareas(),
      nlines(0),
      hpadding(0),
      vpadding(0),
      linespacing(0),
      maxwidth(0),
      totalheight(0),
      textx(0),
      texty(0),
      widths_vector()
{
   
}



DumbText::DumbText(string name) :
      WidgetBase(name),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      text_font(0),
      text(""),
      lines(),
      lineareas(),
      nlines(0),
      hpadding(0),
      vpadding(0),
      linespacing(0),
      maxwidth(0),
      totalheight(0),
      textx(0),
      texty(0),
      widths_vector()

{
   
}
   



void DumbText::DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

   EAGLE_ASSERT(text_font && text_font->Valid());
   
   for (int i = 0 ; i < nlines ; ++i) {
      string s = lines[i];
      Rectangle r = lineareas[i];
      win->DrawTextString(text_font , s , r.X() + xpos , r.Y() + ypos , c , HALIGN_LEFT , VALIGN_TOP);
   }
///   win->DrawMultiLineTextString(text_font , text , tx + xpos , ty + ypos , c , linespacing , halign , valign);
}



void DumbText::SetDrawPos(int xpos , int ypos , bool notify_layout) {
   WidgetBase::SetDrawPos(xpos,ypos,notify_layout);
   Refresh();
}



void DumbText::SetDrawDimensions(int width , int height , bool notify_layout) {
   WidgetBase::SetDrawDimensions(width,height,notify_layout);
   Refresh();
}



void DumbText::SetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetArea(xpos,ypos,width,height,notify_layout);
   Refresh();
}



void DumbText::SetArea(const Rectangle& r , bool notify_layout) {
   DumbText::SetArea(r.X() , r.Y() , r.W() , r.H() , notify_layout);
}



void DumbText::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsExpandFromInner(left,right,top,bottom);
   Refresh();
}



void DumbText::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsContractFromOuter(left,right,top,bottom);
   Refresh();
}



void DumbText::SetupText(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing ,
               std::string textstr , EagleFont* font) {
   
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
   halign = hal;
   valign = val;
   hpadding = hpad;
   vpadding = vpad;
   linespacing = vspacing;
   text = textstr;
   text_font = font;
   Refresh();
}
   


void DumbText::SetText(std::string textstr , EagleFont* font) {
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
   text = textstr;
   text_font = font;
   Refresh();
}



void DumbText::SetFont(EagleFont* font) {
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
   text_font = font;
   Refresh();
}



void DumbText::Refresh() {
   RefreshTextPosition(text_font->Height());
}



void DumbText::Realign(HALIGNMENT hal , VALIGNMENT val) {
   halign = hal;
   valign = val;
   Refresh();
}



using std::endl;
std::ostream& DumbText::DescribeTo(std::ostream& os , Indenter indent) const {
   os <<indent << StringPrintF("Dumb text object \"%s\" at %p." , GetName().c_str() , this) << endl;
   os << indent << PrintAlignment(halign , valign) << endl;
   os << indent << StringPrintF("Using text font (%p) :",this) << endl;
   if (text_font) {
      text_font->DescribeTo(os , indent);
   }
   os << endl;
   os << "There are " << nlines << " lines of text." << endl;
   for (int i = 0 ; i < (int)lines.size() ; ++i) {
      string s = lines[i];
      Rectangle r = lineareas[i];
      os << "Text line " << i + 1 << " is \"" << lines[i] << "\" at ";
      r.DescribeTo(os);
      os << endl;
   }
   os << indent << "HPadding and VPadding are : " << hpadding << " and " << vpadding << endl;
   os << indent << StringPrintF("Linespacing is %d , Max width = is %d , Total height is %d",
                     linespacing , maxwidth , totalheight) << endl;
   WidgetBase::DescribeTo(os , indent);
   return os;
}


