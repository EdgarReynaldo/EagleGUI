
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



#include "Eagle/Gui/Button/GuiButton.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"
#include "Eagle/Area.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/StringWork.hpp"

#include <cmath>
#include <string>
#include <sstream>

using std::stringstream;
using std::string;
using std::endl;



/// -------------------------------     GuiButton     -------------------------------------------




void GuiButton::ResetRadii() {
   // Uses the largest size shape that will fit into the rectangle
   Rectangle r = InnerArea();
   switch (btn_shape) {
      case RECTANGLE_BTN : 
         break;
      case CIRCLE_BTN : 
         rad_a = (r.W()<r.H())?(r.W()/2):(r.H()/2);
         break;
      case ROUNDED_BTN :
         {
            rad_a = hradpercent*InnerArea().W()/2.0;
            rad_b = vradpercent*InnerArea().H()/2.0;
         }
         break;
      case ELLIPSE_BTN : 
         rad_a = r.W()/2;
         rad_b = r.H()/2;
         break;
   }
   SetBgRedrawFlag();
}


void GuiButton::ResetClickArea() {
   switch(btn_shape) {
   case RECTANGLE_BTN :
      SetClickArea(new Rectangle(0 , 0 , InnerArea().W() , InnerArea().H()) , true);
      break;
   case ROUNDED_BTN :
      SetClickArea(new RoundedRectangle(Rectangle(0 , 0 , InnerArea().W() , InnerArea().H()) , rad_a , rad_b) , true);
      break;
   case ELLIPSE_BTN :
      SetClickArea(new Ellipse(0 , 0 , InnerArea().W() , InnerArea().H()) , true);
      break;
   case CIRCLE_BTN :
      SetClickArea(new Circle(InnerArea().W()/2 , InnerArea().H()/2 , rad_a) , true);
      break;
   default :
      break;
   }
}



void GuiButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DrawGuiButtonShape(win , this , xpos , ypos);
   DrawGuiButtonText(win , this , xpos , ypos);
}



void GuiButton::OnAreaChanged() {
   ResetRadii();
   ResetClickArea();
   SetBgRedrawFlag();
}




GuiButton::GuiButton(std::string classname , std::string name) :
      BasicButton(classname , name),
      btn_shape(RECTANGLE_BTN),
      text_font(0),
      text(""),
      rad_a(0),
      rad_b(0),
      hradpercent(0.75),
      vradpercent(0.75)
{
   SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
}



void GuiButton::SetRedrawFlag() {
   SetBgRedrawFlag();
}




void GuiButton::SetButtonType(BUTTON_SHAPE shape , BUTTON_ACTION_TYPE action_type , BUTTON_CLASS button_class) {
   btn_shape = shape;
   btn_action_type = action_type;
   btn_class = button_class;
   ResetRadii();
   ResetClickArea();
   SetBgRedrawFlag();
}



void GuiButton::SetLabel(string label_text) {
   text = label_text;
   SetBgRedrawFlag();
}




void GuiButton::SetRoundingPercent(float hpercent , float vpercent) {
   if (hpercent < 0.0f) {hpercent = 0.0f;}
   if (hpercent > 1.0f) {hpercent = 1.0f;}
   if (vpercent < 0.0f) {vpercent = 0.0f;}
   if (vpercent > 1.0f) {vpercent = 1.0f;}
   hradpercent = hpercent;
   vradpercent = vpercent;
   ResetRadii();
}




void GuiButton::SetFont(EagleFont* textfont) {

   text_font = textfont;
   SetBgRedrawFlag();

}



std::string GuiButton::Text() {
   return text;
}



EagleFont* GuiButton::Font() {
   return text_font;
}



BUTTON_SHAPE GuiButton::ButtonShape() {
   return btn_shape;
}



BUTTON_CLASS GuiButton::ButtonClass() {
   return btn_class;
}



int GuiButton::RadiusA() {
   return rad_a;
}



int GuiButton::RadiusB() {
   return rad_b;
}



std::ostream& GuiButton::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << FullName() << std::endl;
   input_group.DescribeTo(os,indent);
   os << indent << "GuiButton shape = [" << GetButtonShapeText(btn_shape) <<
                   "] GuiButton action type = [" << GetButtonActionTypeText(btn_action_type) << "]" << endl;
   os << indent << "GuiButton state = [" << GetButtonStateText(btn_state) << 
                   "] GuiButton class = [" << GetButtonClassText(btn_class) << "]" << endl;
   os << indent << "Text = \"" << text << "\" , Text font = ";
   if (text_font) {
      text_font->DescribeTo(os);
   }
   else {
      os << "NULL" << endl;
   }
   os << indent << "GuiButton radii (a,b) == (" << rad_a << "," << rad_b <<
         ") , (hradpercent,vradpercent) = (" << hradpercent << "," << vradpercent << ") , spring duration = " << spring_duration << endl;
   os << indent << "Activation state (user , focuskey , pointer) = (" << user_activated <<
         " , " << focuskey_activated << " , " << pointer_activated << ")" << endl;
   os << indent << "Click area (" << click_area << ") = ";
   if (click_area) {
      click_area->DescribeTo(os);
   }
   else {
      os << "Undefined";
   }
   os << StringPrintF(" , delete_area_ptr = %s" , delete_area_ptr?"true":"false");
   os << endl;
   os << indent << StringPrintF("Hover message is %s" , hover_message_enabled?"enabled":"disabled") << endl;
   WidgetBase::DescribeTo(os,indent);
   return os;
}



/// --------------------------------- Global button functions --------------------------------




void DrawGuiButtonShape(EagleGraphicsContext* win , GuiButton* btn , int x , int y) {
   if (!btn && !win) {
      throw EagleException("Null win and Null btn passed to DrawGuiButtonShape.\n");
   }
   else if (!btn) {
      throw EagleException("Null btn passed to DrawGuiButtonShape.\n");
   }
   else if (!win) {
      throw EagleException("Null win passed to DrawGuiButtonShape.\n");
   }
   bool up = (btn->ButtonState()%2) == 0;
   int rad_a = btn->RadiusA();
   int rad_b = btn->RadiusB();
///   int rad_b = btn->RadiusB();
   Rectangle r = btn->InnerArea();
   r.MoveBy(x,y);
   WidgetColorset wc = btn->WidgetColors();
   /// TODO : Add hover coloring?
   if (btn->Flags().FlagOn(HOVER)) {
      switch(btn->ButtonShape()) {
         case RECTANGLE_BTN : 
            up?
            r.DrawGuiRectUp(win , wc[HVRCOL] , wc[SDCOL]):
            r.DrawGuiRectDown(win , wc[MGCOL] , wc[SDCOL]);
            break;
         case CIRCLE_BTN : 
            up?
            r.DrawGuiCircleUp(win , rad_a , wc[HVRCOL] , wc[SDCOL]):
            r.DrawGuiCircleDown(win , rad_a , wc[MGCOL] , wc[SDCOL]);
            break;
         case ROUNDED_BTN :
            up?
            r.DrawGuiRoundedUp(win , rad_a , rad_b , wc[HVRCOL] , wc[SDCOL]):
            r.DrawGuiRoundedDown(win , rad_a , rad_b , wc[MGCOL] , wc[SDCOL]);
            break;
         case ELLIPSE_BTN : 
            up?
            r.DrawGuiEllipseUp(win , wc[HVRCOL] , wc[SDCOL]):
            r.DrawGuiEllipseDown(win , wc[MGCOL] , wc[SDCOL]);
            break;
      }
   }
   else {
      switch(btn->ButtonShape()) {
         case RECTANGLE_BTN : 
            up?
            r.DrawGuiRectUp(win , wc[FGCOL] , wc[MGCOL]):
            r.DrawGuiRectDown(win , wc[MGCOL] , wc[BGCOL]);
            break;
         case CIRCLE_BTN : 
            up?
            r.DrawGuiCircleUp(win , rad_a , wc[FGCOL] , wc[MGCOL]):
            r.DrawGuiCircleDown(win , rad_a , wc[MGCOL] , wc[BGCOL]);
            break;
         case ROUNDED_BTN :
            up?
            r.DrawGuiRoundedUp(win , rad_a , rad_b , wc[FGCOL] , wc[MGCOL]):
            r.DrawGuiRoundedDown(win , rad_a , rad_b , wc[MGCOL] , wc[BGCOL]);
            break;
         case ELLIPSE_BTN : 
            up?
            r.DrawGuiEllipseUp(win , wc[FGCOL] , wc[MGCOL]):
            r.DrawGuiEllipseDown(win , wc[MGCOL] , wc[BGCOL]);
            break;
      }
   }
}



void DrawGuiButtonText(EagleGraphicsContext* win , GuiButton* btn , int x , int y) {
   if (!btn && !win) {
      throw EagleException("Null win and Null btn passed to DrawGuiButtonText.\n");
   }
   else if (!btn) {
      throw EagleException("Null btn passed to DrawGuiButtonText.\n");
   }
   else if (!win) {
      throw EagleException("Null win passed to DrawGuiButtonText.\n");
   }
   
   EagleFont* text_font = btn->Font();
   string text = btn->Text();
   
   if (text.length()) {
      EAGLE_ASSERT(text_font);
      if (!text_font) {
         throw EagleException(StringPrintF("GuiButton object has text (\"%s\" but no associated font has been set!!!\n",text.c_str()));
      }
      Rectangle r = btn->InnerArea();
      int tx = x + r.X() + r.W()/2;
      int ty = y + r.Y() + r.H()/2;
      
      win->DrawGuiTextString(text_font , text.c_str() , tx , ty , btn->GetColor(TXTCOL) , HALIGN_CENTER , VALIGN_CENTER);
   }
   
}




