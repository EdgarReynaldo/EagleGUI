



#include "Eagle/Gui/Text/BasicText.hpp"


#include <iostream>
#include <string>
using namespace std;


#include "Eagle/Platform.hpp"
#include "Eagle/Gui/Text.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/StringWork.hpp"

#include "Eagle/Gui/Decorators/TextDecorator.hpp"

#include <cstdlib>


void BasicText::RefreshTextPosition() {

   EAGLE_ASSERT(text_font);
   EAGLE_ASSERT(text_font->Valid());
   lines = SplitByNewLines(text);
   nlines = lines.size();
   
   int lineheight = fontheight + linespacing;
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



int BasicText::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int BasicText::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void BasicText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DrawText(win , xpos , ypos , WCols()[TXTCOL]);
}



int BasicText::PrivateUpdate(double tsec) {
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
BasicText::BasicText() :
      WidgetBase(StringPrintF("Basic Text object at %p" , this)),
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
      widths_vector(),
      scale_to_fit(false),
      scaling_transform(),
      inverse_scaling_transform()
{
   
}



BasicText::BasicText(string name) :
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
      fontheight(0),
      totalheight(0),
      textx(0),
      texty(0),
      widths_vector(),
      scale_to_fit(false),
      scaling_transform(),
      inverse_scaling_transform()
{
   
}
   



void BasicText::DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

   EAGLE_ASSERT(text_font && text_font->Valid());
   
   Transformer* transformer = win->GetTransformer();
   
   if (scale_to_fit) {
      
      scaling_transform = transformer->GetIdentityTransform();
      
      scaling_transform.Translate(-textx , -texty);
      scaling_transform.Scale((float)InnerArea().W()/maxwidth , (float)InnerArea().H()/totalheight);
      scaling_transform.Translate(InnerArea().X() , InnerArea().Y());
      
      scaling_transform *= transformer->GetViewTransform();
      
      transformer->PushViewTransform(scaling_transform);
   }
   
   
   win->HoldDrawing();
   for (int i = 0 ; i < nlines ; ++i) {
      string s = lines[i];
      Rectangle r = lineareas[i];
      win->DrawTextString(text_font , s , r.X() + xpos , r.Y() + ypos , c , HALIGN_LEFT , VALIGN_TOP);
   }
   win->ReleaseDrawing();
   
   if (scale_to_fit) {
      win->GetTransformer()->PopViewTransform();
   }
///   win->DrawMultiLineTextString(text_font , text , tx + xpos , ty + ypos , c , linespacing , halign , valign);
}



void BasicText::SetWidgetArea(int x , int y , int w , int h , bool notify_layout) {
   WidgetBase::SetWidgetArea(x,y,w,h,notify_layout);
   Refresh();
}


void BasicText::ShrinkWrap() {
   
   scale_to_fit = false;
   
   WidgetArea a = area;
   a.SetInnerArea(textx - hpadding , texty - vpadding , maxwidth + 2*hpadding , totalheight + 2*vpadding);
   
   WidgetBase::SetWidgetArea(a.OuterArea());
}



void BasicText::ScaleToFit(bool scale) {
   scale_to_fit = scale;
   SetRedrawFlag();
}



void BasicText::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsExpandFromInner(left,right,top,bottom);
   Refresh();
}



void BasicText::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsContractFromOuter(left,right,top,bottom);
   Refresh();
}



void BasicText::SetupText(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing ,
               std::string textstr , EagleFont* font) {
   
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
   halign = hal;
   valign = val;
   hpadding = hpad;
   vpadding = vpad;
   linespacing = vspacing;
   SetText(textstr , font);
}
   


void BasicText::SetText(std::string textstr , EagleFont* font) {
   text = textstr;
   text_font = font;
   if (text_font) {
      fontheight = text_font->Height();
   }
   else {
      fontheight = 0;
   }
   Refresh();
   
   TextDecorator* text_decorator = dynamic_cast<TextDecorator*>(GetDecoratorParent());
   
   if (text_decorator) {
      text_decorator->RepositionText();
   }
}



void BasicText::SetText(std::string textstr) {
   SetText(textstr , text_font);
}



void BasicText::SetFont(EagleFont* font) {
   SetText(text , font);
}



void BasicText::Refresh() {
   if (text_font) {
      RefreshTextPosition();
   }
///   else {
///      EagleInfo() << StringPrintF("INFO : BasicText::Refresh called on object %s without an active text_font.\n" , GetName().c_str());
///   }
}



void BasicText::Realign(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad) {
   
   if (hpad < 0) {hpad = 0;}
   if (vpad < 0) {vpad = 0;}
   
   halign = hal;
   valign = val;
   hpadding = hpad;
   vpadding = vpad;
   Refresh();
}



void BasicText::SetLineSpacing(int vspacing) {
   linespacing = vspacing;
   Refresh();
}



int BasicText::NLines() {return nlines;}



Rectangle BasicText::LineArea(int linenum) {
   EAGLE_ASSERT(linenum >= 0 && linenum < nlines);
   return lineareas[linenum];
}



std::string BasicText::LineString(int linenum) {
   EAGLE_ASSERT(linenum >= 0 && linenum < nlines);
   return lines[linenum];
}



using std::endl;
std::ostream& BasicText::DescribeTo(std::ostream& os , Indenter indent) const {
   os <<indent << StringPrintF("Basic text object \"%s\" at %p." , GetNameCStr() , this) << endl;
   os << indent << PrintAlignment(halign , valign) << endl;
   os << indent << StringPrintF("Using text font (%p) :",this) << endl;
   if (text_font) {
      text_font->DescribeTo(os , indent);
   }
   os << endl;
   os << indent << StringPrintF("There %s %d line%s of text." , (nlines==1)?"is":"are" , nlines , (nlines==1)?"":"s") << endl;
   for (int i = 0 ; i < (int)lines.size() ; ++i) {
      string s = lines[i];
      Rectangle r = lineareas[i];
      os << indent << "Text line " << i + 1 << " is \"" << lines[i] << "\" at ";
      r.DescribeTo(os);
      os << endl;
   }
   os << indent << "HPadding and VPadding are : " << hpadding << " and " << vpadding << endl;
   os << indent << StringPrintF("Linespacing is %d , Max width is %d , Total height is %d",
                     linespacing , maxwidth , totalheight) << endl;
   WidgetBase::DescribeTo(os , indent);
   return os;
}


