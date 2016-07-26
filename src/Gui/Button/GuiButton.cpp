
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2015+ by Edgar Reynaldo
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
   Rectangle r = area.InnerArea();
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



/**
   BUTTON_SHAPE btn_shape;

   EagleFont* text_font;
   std::string text;

   /// Button type determines hit detection properties as well
   // Rectangle button uses dimensions passed in constructor call
   int rad_a;// Primary radius for circles and the horizontal radius for ellipse type buttons,
               // as well as the corner radius for rounded rectangular buttons
   int rad_b;// Secondary vertical radius, only for ellipses

   float hradpercent;// for ROUNDED_BTN type buttons
   float vradpercent;// for ROUNDED_BTN type buttons
   //*/
GuiButton::GuiButton() :
      BasicButton(StringPrintF("GuiButton object at %p" , this)),
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



GuiButton::GuiButton(string name) :
      BasicButton(name),
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

int GuiButton::PrivateCheckInputs() {

   UINT retmsg = DIALOG_OKAY;
   bool activated = false;
   bool released = false;

   int msx = mouse_x;
   int msy = mouse_y;
   WidgetHandler* whandler = 0;
   
   if (wparent) {
      whandler = dynamic_cast<WidgetHandler*>(wparent);
   }
   if (whandler) {
      msx = whandler->GetMouseX();
      msy = whandler->GetMouseY();
   }
///   int msx = mouse_x - AbsParentX();// This won't work becase it doesn't include the parent gui's camera position
///   int msy = mouse_y - AbsParentY();// see above

   /// Track whether the button is still being held since the last activation
   if (user_activated || focuskey_activated || pointer_activated) {
      /// Look for input releases
      if (user_activated) {
         Input click = input_group.FindClickInput();
         if (click != input_key_press(EAGLE_KEY_NONE)) {
            Input release(click.Source() , RELEASE , click.Value());
            if (release) {
               released = true;
            }
         }
      }
      if (focuskey_activated) {
         if (input_key_release(EAGLE_KEY_SPACE) || input_key_release(EAGLE_KEY_ENTER)) {released = true;}
      }
      if (pointer_activated) {
         if (input_mouse_release(LMB)) {released = true;}
      }
      if (released) {
         SetButtonState(Flags() & HOVER , true);
         user_activated = false;
         focuskey_activated = false;
         pointer_activated = false;
      } else {
         down_time_left = spring_duration;
         QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD);
      }
      return DIALOG_OKAY;
   }


   if (input_group) {
         
      activated = true;
      if (btn_action_type == SPRING_BTN) {user_activated = true;}
   
   } else if ((Flags() & HASFOCUS) && (input_key_press(EAGLE_KEY_SPACE) || input_key_press(EAGLE_KEY_ENTER))) {
      
      activated = true;
      if (btn_action_type == SPRING_BTN) {focuskey_activated = true;}
      
   } else if (input_mouse_press(LMB) && area.InnerArea().Contains(msx,msy)) {
      
      if (click_area) {
         if (click_area->Contains(msx - area.InnerArea().X() , msy - area.InnerArea().Y())) {
            activated = true;
            if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
         }
      }
      else {
         Rectangle r = area.InnerArea();
         switch (btn_shape) {
            case RECTANGLE_BTN :
               activated = true;
               if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
               break;
            case CIRCLE_BTN :
               {
                  Circle c(r.X() + r.W()/2 , r.Y() + r.H()/2 , rad_a);
                  if (c.Contains(msx,msy)) {
                     activated = true;
                     if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
                  }
               }
               break;
            case ROUNDED_BTN :
               {
                  RoundedRectangle rr(InnerArea() , rad_a , rad_b);
                  if (rr.Contains(msx,msy)) {
                     activated = true;
                     if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
                  }
               }
               break;
            case ELLIPSE_BTN :
               {
                  Ellipse e(InnerArea());
                  if (e.Contains(msx,msy)) {
                     activated = true;
                     if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
                  }
               }
               break;
         }
      }
   }
   
   if (activated) {// click or key press has activated the button action
      retmsg |= DIALOG_TAKE_FOCUS;
      retmsg |= DIALOG_INPUT_USED;
      bool up = btn_state%2 == 0;
      switch (btn_action_type) {
         case SPRING_BTN :
            if (up) {
               up = false;
            }
            break;
         case TOGGLE_BTN :
            up = !up;
            break;
         default : throw EagleError("GuiButton::PrivateCheckInputs - btn_action_type unknown");break;
      }
      SetButtonState(Flags() & HOVER , up);
      if (WidgetBase::Flags() & ALLOW_CLOSE) {
         retmsg |= DIALOG_CLOSE;
      }
   }
   return retmsg;
   
}



void GuiButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DrawGuiButtonShape(win , this , xpos , ypos);
   DrawGuiButtonText(win , this , xpos , ypos);
   ClearRedrawFlag();
}



int GuiButton::PrivateUpdate(double tsec) {
   UINT retmsg = DIALOG_OKAY;
   
   if (btn_action_type == SPRING_BTN) {
      bool up = btn_state%2 == 0;
      bool hover = Flags() & HOVER;
      if (!up && !user_activated && !focuskey_activated && !pointer_activated) {
         down_time_left -= tsec;
         if (down_time_left <= 0.0) {
            down_time_left = spring_duration;
            up = true;
            SetButtonState(hover , up);
         }
      }
   }
   return retmsg;
}


void GuiButton::QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages) {
/**
   WidgetMsg msg(widget_address , widget_topic , messages);
   EagleLog() << "Queueing user message [" << msg << "]. Widget state is currently :" << endl;
   this->DescribeTo(EagleLog());
   EagleLog() << endl;
*/
   WidgetBase::QueueUserMessage(widget_address , widget_topic , messages);
}

/* TODO OLD REMOVE
WidgetMsg GuiButton::CheckInputs(int msx , int msy)// Pass it the mouse position relative to it's drawing target,
                                                // except for WidgetHandlers
                                                // Hover is taken care of by WidgetHandlers, if you need it to
                                                // do something, override the virtual SetHoverState function.
{
   
   if (!(WidgetBase::Flags() & ENABLED)) {
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_DISABLED);
   }
   
   UINT retmsg = DIALOG_OKAY;
   bool activated = false;
   bool released = false;

   /// Track whether the button is still being held since the last activation
   if (user_activated || focuskey_activated || pointer_activated) {
      /// Look for input releases
      if (user_activated) {
         Input click = input_group.FindClickInput();
         if (click != input_key_press(KEY_NONE)) {
            Input release(click.Source() , RELEASE , click.Value());
            if (release) {
               released = true;
            }
         }
      }
      if (focuskey_activated) {
         if (input_key_release(KEY_SPACE) || input_key_release(KEY_ENTER)) {released = true;}
      }
      if (pointer_activated) {
         if (input_mouse_release(LMB)) {released = true;}
      }
      if (released) {
         QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);
         user_activated = false;
         focuskey_activated = false;
         pointer_activated = false;
         retmsg |= DIALOG_REDRAW_ME;
         SetRedrawFlag();
      } else {
         QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD);
      }
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
   }


   if (input_group) {
      activated = true;
      if (action_type == SPRING_BTN) {user_activated = true;}
   } else if ((WidgetBase::Flags() & HASFOCUS) && (input_key_press(KEY_SPACE) || input_key_press(KEY_ENTER))) {
      activated = true;
      if (action_type == SPRING_BTN) {focuskey_activated = true;}
   } else if (input_mouse_press(LMB) && area.Contains(msx,msy)) {
      if (click_area) {
         if (click_area->Contains(msx - area.X() , msy - area.Y())) {
            activated = true;
            if (action_type == SPRING_BTN) {pointer_activated = true;}
         }
      }
      else {
         switch (btn_shape) {
            case RECTANGLE_BTN :
               activated = true;
               if (action_type == SPRING_BTN) {pointer_activated = true;}
               break;
            case CIRCLE_BTN :
               {
                  Circle c(area.X() + area.W()/2 , area.Y() + area.H()/2 , rad_a);
                  if (c.Contains(msx,msy)) {
                     activated = true;
                     if (action_type == SPRING_BTN) {pointer_activated = true;}
                  }
               }
               break;
            case ROUNDED_BTN :
               // Lazy hit detection since a rounded rectangle is so close to a rectangle anyway.
               activated = true;
               if (action_type == SPRING_BTN) {pointer_activated = true;}
               break;
            case ELLIPSE_BTN :
               {
                  int xd = msx - (area.X() + area.W()/2);
                  int yd = msy - (area.Y() + area.H()/2);
                  double dist_sq = xd*xd + yd*yd;
                  double angle = atan2(yd,xd);
                  double exd = (double)rad_a*cos(angle);
                  double eyd = (double)rad_b*sin(angle);
                  double ellp_dist = exd*exd + eyd*eyd;
                  if (dist_sq <= ellp_dist) {
                     activated = true;
                     if (action_type == SPRING_BTN) {pointer_activated = true;}
                  }
               }
               break;
         }
      }
   }
   
   if (activated) {// click or key press has activated the button action
      retmsg |= DIALOG_TAKE_FOCUS;
      retmsg |= DIALOG_REDRAW_ME;
      retmsg |= DIALOG_INPUT_USED;
      SetBgRedrawFlag();
      switch (action_type) {
         case SPRING_BTN :
            if (up) {
               up = false;
               QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
               if (WidgetBase::Flags() & ALLOW_CLOSE) {
                  retmsg |= DIALOG_CLOSE;
               }
            }
            break;
         case TOGGLE_BTN :
            up = !up;
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
            if (WidgetBase::Flags() & ALLOW_CLOSE) {
               retmsg |= DIALOG_CLOSE;
            }
            break;
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}


/// TODO OLD REMOVE
void GuiButton::DisplayOn(BITMAP* bmp , int x , int y)/// DisplayOn should always draw, and always 
                                                   /// clear the NEEDS_REDRAW flag.
{
   DrawGuiButtonRectangle(bmp , x , y);
   DrawGuiButtonText(bmp , x , y);
   ClearRedrawFlag();
}

*/


/* TODO OLD REMOVE
WidgetMsg GuiButton::Update (double tsec) // logic handling with animation support
{
   UINT retmsg = DIALOG_OKAY;
   if (!(WidgetBase::Flags() & ENABLED)) {
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_DISABLED);
   }
   
   if (action_type == SPRING_BTN) {
      if (!up && !user_activated && !focuskey_activated && !pointer_activated) {
         dn_time_left -= tsec;
         if (dn_time_left <= 0.0) {
            dn_time_left = spring_duration;
            up = true;
            retmsg |= DIALOG_REDRAW_ME;
            SetBgRedrawFlag();
         }
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}
*/



void GuiButton::SetRedrawFlag() {
   SetBgRedrawFlag();
}



void GuiButton::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   ResetRadii();
   SetBgRedrawFlag();
}



void GuiButton::SetButtonType(BUTTON_SHAPE shape , BUTTON_ACTION_TYPE action_type , BUTTON_CLASS button_class) {
   btn_shape = shape;
   btn_action_type = action_type;
   btn_class = button_class;
   ResetRadii();
   SetBgRedrawFlag();
}



void GuiButton::SetLabel(string label_text) {
   text = label_text;
   SetName(StringPrintF("\"%s\" %s at %p.", GetName().c_str() , GetWidgetClassName().c_str() , this));
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



std::string GuiButton::GetWidgetClassName() {
   return "GuiButton";
}




std::ostream& GuiButton::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("GuiButton object %s at %p :",GetName().c_str(),this) << endl;
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
      throw EagleError("Null win and Null btn passed to DrawGuiButtonShape.\n");
   }
   else if (!btn) {
      throw EagleError("Null btn passed to DrawGuiButtonShape.\n");
   }
   else if (!win) {
      throw EagleError("Null win passed to DrawGuiButtonShape.\n");
   }
   bool up = (btn->ButtonState()%2) == 0;
   int rad_a = btn->RadiusA();
   int rad_b = btn->RadiusB();
///   int rad_b = btn->RadiusB();
   Rectangle r = btn->InnerArea();
   r.MoveBy(x,y);
   switch(btn->ButtonShape()) {
      case RECTANGLE_BTN : 
         up?
         r.DrawGuiRectUp(win , btn->WCols()[FGCOL] , btn->WCols()[SDCOL]):
         r.DrawGuiRectDown(win , btn->WCols()[MGCOL] , btn->WCols()[SDCOL]);
         break;
      case CIRCLE_BTN : 
         up?
         r.DrawGuiCircleUp(win , rad_a , btn->WCols()[FGCOL] , btn->WCols()[SDCOL]):
         r.DrawGuiCircleDown(win , rad_a , btn->WCols()[MGCOL] , btn->WCols()[SDCOL]);
         break;
      case ROUNDED_BTN :
         up?
         r.DrawGuiRoundedUp(win , rad_a , rad_b , btn->WCols()[FGCOL] , btn->WCols()[SDCOL]):
         r.DrawGuiRoundedDown(win , rad_a , rad_b , btn->WCols()[MGCOL] , btn->WCols()[SDCOL]);
         break;
      case ELLIPSE_BTN : 
         up?
         r.DrawGuiEllipseUp(win , btn->WCols()[FGCOL] , btn->WCols()[SDCOL]):
         r.DrawGuiEllipseDown(win , btn->WCols()[MGCOL] , btn->WCols()[SDCOL]);
         break;
   }
}



void DrawGuiButtonText(EagleGraphicsContext* win , GuiButton* btn , int x , int y) {
   if (!btn || !win) {
      throw EagleError("Null win and Null btn passed to DrawGuiButtonText.\n");
   }
   else if (!btn) {
      throw EagleError("Null btn passed to DrawGuiButtonText.\n");
   }
   else if (!win) {
      throw EagleError("Null win passed to DrawGuiButtonText.\n");
   }
   
   EagleFont* text_font = btn->Font();
   string text = btn->Text();
   
   if (text.length()) {
      EAGLE_ASSERT(text_font);
      if (!text_font) {
         throw EagleError(StringPrintF("GuiButton object has text (\"%s\" but no associated font has been set!!!\n",text.c_str()));
      }
      Rectangle r = btn->InnerArea();
      int tx = x + r.X() + r.W()/2;
      int ty = y + r.Y() + r.H()/2;
      
      win->DrawGuiTextString(text_font , text.c_str() , tx , ty , btn->WCols()[TXTCOL] , HALIGN_CENTER , VALIGN_CENTER);
   }
   
}






/// --------------------------------     HoverGuiButton     --------------------------------

/*

HoverGuiButton::HoverGuiButton(std::string name) :
      GuiButton(name)
{}



HoverGuiButton::HoverGuiButton(std::string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont , 
                         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags) :
      GuiButton(name , shape , atype , textfont , label , input , position , wflags)
{}



void HoverGuiButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & HOVER) {
      if (up) {
         Rectangle r = area;
         r.MoveBy(x,y);
         switch (btn_shape) {
            case RECTANGLE_BTN : 
               r.DrawGuiRectUp(bmp , WCols()[BGCOL] , WCols()[SDCOL]);
               break;
            case CIRCLE_BTN : 
               r.DrawGuiCircleUp(bmp , rad_a , WCols()[BGCOL] , WCols()[SDCOL]);
               break;
            case ROUNDED_BTN :
               r.DrawGuiRoundedUp(bmp , rad_a , WCols()[BGCOL] , WCols()[SDCOL]);
               break;
            case ELLIPSE_BTN : 
               r.DrawGuiEllipseUp(bmp , WCols()[BGCOL] , WCols()[SDCOL]);
               break;
         }
      }
      else {
         GuiButton::DisplayOn(bmp , x , y);
      }
   }
   else {
      GuiButton::DisplayOn(bmp , x , y);
   }
}


   
void HoverGuiButton::SetHoverState (bool state) {
   WidgetBase::SetHoverState(state);
   SetRedrawFlag();
}
  
*/











/* TODO : OLD REMOVE



#include "Eagle/Gui/GuiButton.hpp"
#include "Eagle/InputHandler.hpp"



const unsigned int TOPIC_BUTTON = NextFreeTopicId();



GuiButtonBase::GuiButtonBase() :
      down(false),
      hover(false),
      selected(false),
      upimage(0),
      downimage(0),
      hoverimage(0),
      selectedimage(0),
      text(""),
      font(0)
{
   outer_color = EagleColor(0,64,64);
   down_color = EagleColor(0,127,127);
   up_color = EagleColor(0,0,255);
   hover_color = EagleColor(0,192,192);
   selected_color = EagleColor(0,255,0);
   text_color = EagleColor(255,255,255);
   text_color2 = EagleColor(0,0,255);
}



GuiButtonBase::GuiButtonBase(std::string text_str , EagleFont* text_font) :
      down(false),
      hover(false),
      selected(false),
      upimage(0),
      downimage(0),
      hoverimage(0),
      selectedimage(0),
      text(text_str),
      font(text_font)
{
   outer_color = EagleColor(0,64,64);
   down_color = EagleColor(0,127,127);
   up_color = EagleColor(0,0,255);
   hover_color = EagleColor(0,192,192);
   selected_color = EagleColor(0,255,0);
   text_color = EagleColor(255,255,255);
   text_color2 = EagleColor(0,0,255);
}



int GuiButtonBase::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int GuiButtonBase::CheckInputs() {
   if (input_mouse_release(LMB) && down) {
      QueueUserMessage(this , TOPIC_BUTTON , BUTTON_RELEASED);
      down = false;
      SetRedrawFlag();
   }
   if (area.OuterArea().Contains(mouse_x , mouse_y)) {
       if (!hover) {
         QueueUserMessage(this , TOPIC_BUTTON , BUTTON_HOVER);
         SetRedrawFlag();
       }
       hover = true;
      if (input_mouse_press(LMB)) {
         QueueUserMessage(this , TOPIC_BUTTON , BUTTON_CLICKED);
         down = true;
         SetRedrawFlag();
      }
   }
   else {
      if (hover) {
         hover = false;
         SetRedrawFlag();
      }
   }
   return DIALOG_OKAY;
}



void GuiButtonBase::Display(EagleGraphicsContext* win , int xpos , int ypos) {


//   virtual void DrawTextString(EagleFont* font , std::string s , float x , float y , EagleColor c ,
//                               TEXT_HDRAWING_FLAGS halign = DRAW_TEXT_LEFT ,
//                               TEXT_VDRAWING_FLAGS valign = DRAW_TEXT_TOP)=0;

//   virtual void DrawFilledRoundedRectangle(float x , float y , float w , float h , float rx , float ry , EagleColor c)=0;



   Rectangle r = area.OuterArea();
   Rectangle r2 = area.InnerArea();
   r.MoveBy(xpos,ypos);
   r2.MoveBy(xpos,ypos);
   
   
   if (flags & NEEDS_REDRAW) {
         
      if (selected) {
         win->DrawFilledRoundedRectangle(r.X() , r.Y() , r.W() , r.H() , 10 , 5 , outer_color);
         win->DrawFilledRoundedRectangle(r2.X() , r2.Y() , r2.W() , r2.H() , 10 , 5 , selected_color);
         win->DrawTextString(font , text , r.X() + r.W()/2 , r.Y() + r.H()/2 , text_color , DRAW_TEXT_CENTER , DRAW_TEXT_VCENTER);
      }
      if (down) {
         win->DrawFilledRoundedRectangle(r.X() , r.Y() , r.W() , r.H() , 10 , 5 , outer_color);
         win->DrawFilledRoundedRectangle(r2.X() , r2.Y() , r2.W() , r2.H() , 10 , 5 , down_color);
         win->DrawTextString(font , text , r.X() + r.W()/2 , r.Y() + r.H()/2 , text_color , DRAW_TEXT_CENTER , DRAW_TEXT_VCENTER);
      }
      else {
         if (hover) {
            win->DrawFilledRoundedRectangle(r.X() , r.Y() , r.W() , r.H() , 10 , 5 , outer_color);
            win->DrawFilledRoundedRectangle(r2.X() , r2.Y() , r2.W() , r2.H() , 10 , 5 , hover_color);
            win->DrawTextString(font , text , r.X() + r.W()/2 , r.Y() + r.H()/2 , text_color , DRAW_TEXT_CENTER , DRAW_TEXT_VCENTER);
         }
         else {
            win->DrawFilledRoundedRectangle(r.X() , r.Y() , r.W() , r.H() , 10 , 5 , outer_color);
            win->DrawFilledRoundedRectangle(r2.X() , r2.Y() , r2.W() , r2.H() , 10 , 5 , up_color);
            win->DrawTextString(font , text , r.X() + r.W()/2 , r.Y() + r.H()/2 , text_color2 , DRAW_TEXT_CENTER , DRAW_TEXT_VCENTER);
         }
      }
   }
   
   ClearRedrawFlag();
}



int GuiButtonBase::Update(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}



/// Highlight GuiButton


void HighlightGuiButton::Display(EagleGraphicsContext* win , int xpos , int ypos) {
   
   Rectangle r = area.OuterArea();
   r.MoveBy(xpos , ypos);
   
   if (hover) {
      if (selected) {
         win->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , selected_color);
      }
      win->DrawRectangle(r.X() + 5 , r.Y() + 5  , r.W() - 10 , r.H() - 10 , 10 , hover_color);
   }
   else {
      
   }
   win->DrawTextString(font , text , r.X() + r.W()/2 , r.Y() + r.H()/2 , text_color , DRAW_TEXT_CENTER , DRAW_TEXT_VCENTER);
}

*/





