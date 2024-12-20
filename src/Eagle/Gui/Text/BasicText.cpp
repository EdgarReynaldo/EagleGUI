
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




#include "Eagle/Gui/Text/BasicText.hpp"


#include <iostream>
#include <string>
using namespace std;


#include "Eagle/Platform.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/StringWork.hpp"

#include <cstdlib>


void BasicText::RefreshTextPosition() {

   lines = SplitByNewLinesNoChomp(text);
   nlines = lines.size();
   if (nlines < 1) {return;}

   EAGLE_ASSERT(text_font);
   EAGLE_ASSERT(text_font->Valid());
   
   int lineheight = fontheight + linespacing;
   maxwidth = 0;
   totalheight = fontheight*nlines + linespacing*(nlines-1);

   lineareas.clear();
   

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
      lineareas.push_back(Rectangle(x,y,w,fontheight));
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
   DrawText(win , xpos , ypos , GetColor(TXTCOL));
}



int BasicText::PrivateUpdate(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}



void BasicText::OnAreaChanged() {
   Refresh();
}



BasicText::BasicText(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
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
   



BasicText::BasicText(std::string objname) :
      WidgetBase("BasicText" , objname),
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



BasicText::BasicText(EagleFont* font , std::string label , HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing) :
      WidgetBase("BasicText" , label + "_label"),
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
   SetupText(label , font , hal , val , hpad , vpad , vspacing);
}



void BasicText::SetRedrawFlag() {
   SetBgRedrawFlag();
}



void BasicText::DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

   if (nlines < 1) {return;}
   
   EAGLE_ASSERT(text_font && text_font->Valid());
   
   if (scale_to_fit) {
      
      scaling_transform = win->GetTransformer()->CreateTransform();
      
      scaling_transform.Translate(-textx , -texty);
      scaling_transform.Scale((float)InnerArea().W()/maxwidth , (float)InnerArea().H()/totalheight);
      scaling_transform.Translate(InnerArea().X() , InnerArea().Y());
      
      scaling_transform *= win->GetTransformer()->GetViewTransform();/// Respect the current transform
      
      win->PushViewTransform(scaling_transform);
   }
   
   
   win->HoldDrawing();
   for (int i = 0 ; i < nlines ; ++i) {
      string s = lines[i];
      Rectangle r = lineareas[i];
      win->DrawGuiTextString(text_font , s , r.X() + xpos , r.Y() + ypos , c , HALIGN_LEFT , VALIGN_TOP);
   }
   win->ReleaseDrawing();
   
   if (scale_to_fit) {
      win->PopViewTransform();
   }
///   win->DrawMultiLineTextString(text_font , text , tx + xpos , ty + ypos , c , linespacing , halign , valign);
}



void BasicText::ShrinkWrap() {
   
   scale_to_fit = false;
   
   WidgetArea wa = GetWidgetArea();
   wa.SetInnerArea(Rectangle(textx - hpadding , texty - vpadding , maxwidth + 2*hpadding , totalheight + 2*vpadding));
   
   SetWidgetArea(wa);
}



void BasicText::ScaleToFit(bool scale) {
   scale_to_fit = scale;
   SetRedrawFlag();
}



void BasicText::SetupText(std::string textstr , EagleFont* font , HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing) {

   text = textstr;
   if (!font) {
      font = text_font;
   }
   else {
      text_font = font;
   }
   halign = hal;
   valign = val;
   hpadding = hpad;
   vpadding = vpad;
   linespacing = vspacing;
   if (text_font) {
      fontheight = text_font->Height();
   }
   else {
      fontheight = 0;
   }
   Refresh();
}
   


void BasicText::Refresh() {
   if (text_font) {
      RefreshTextPosition();
   }
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



EagleFont* BasicText::GetFont() {
   return text_font;
}



void BasicText::SetPreferredSize(int pw , int ph) {
   if (pw == -1) {
      pw = maxwidth + 2*hpadding;
   }
   if (ph == -1) {
      ph = totalheight + 2*vpadding*(nlines-1);
   }
   WidgetBase::SetPreferredSize(pw,ph);
}



using std::endl;
std::ostream& BasicText::DescribeTo(std::ostream& os , Indenter indent) const {
   os <<indent << FullName() << endl;
   os << indent << PrintAlignment(halign , valign) << endl;
   os << indent << StringPrintF("Using text font (%p) :" , (void*)text_font) << endl;
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


