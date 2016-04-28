
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


#include "Eagle/Gui/Button.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"
#include "Eagle/Area.hpp"
#include "Eagle/StringWork.hpp"

#include <cmath>
#include <string>
#include <sstream>

using std::stringstream;
using std::string;
using std::endl;

double SPRING_BTN_DURATION = 0.25;
double BTN_ROUNDNESS = 0.4;



bool ButtonUp(BUTTON_STATE state) {
   return (state == BUTTON_UP || state == BUTTON_HOVER_UP);
}



bool ButtonHover(BUTTON_STATE state) {
   return (state == BUTTON_HOVER_UP || state == BUTTON_HOVER_DOWN);
}



string GetButtonShapeText(BUTTON_SHAPE shape) {
   stringstream ss;
   switch (shape) {
      case RECTANGLE_BTN : ss << "RECTANGLE_BTN";break;
      case CIRCLE_BTN : ss << "CIRCLE_BTN";break;
      case ROUNDED_BTN : ss << "ROUNDED_BTN";break;
      case ELLIPSE_BTN : ss << "ELLIPSE_BTN";break;
      default : ss << "Unknown button shape";break;
   };
   return ss.str();
}



string GetButtonActionTypeText(BUTTON_ACTION_TYPE type) {
   stringstream ss;
   switch (type) {
      case SPRING_BTN : ss << "SPRING_BTN";break;
      case TOGGLE_BTN : ss << "TOGGLE_BTN";break;
      default : ss << "Unknown button action type";break;
   };
   return ss.str();
}



string GetButtonClassText(BUTTON_CLASS _class) {
   stringstream ss;
   switch (_class) {
      case BUTTON_CLASS_PLAIN : ss << "BUTTON_CLASS_PLAIN";break;
      case BUTTON_CLASS_HOVER : ss << "BUTTON_CLASS_HOVER";break;
      default : ss << "Unknown button class";break;
   };
   return ss.str();
}



string GetButtonStateText(BUTTON_STATE state) {
   stringstream ss;
   switch (state) {
      case BUTTON_UP : ss << "BUTTON_UP";break;
      case BUTTON_DOWN : ss << "BUTTON_DOWN";break;
      case BUTTON_HOVER_UP : ss << "BUTTON_HOVER_UP";break;
      case BUTTON_HOVER_DOWN : ss << "BUTTON_HOVER_DOWN";break;
      default : ss << "Unknown button state";break;
   };
   return ss.str();
}



/// -------------------------------     Button     -------------------------------------------



void Button::SetButtonState(bool hover , bool up) {
   bool oldhover = Flags() & HOVER;
   bool oldup = (btn_state % 2 == 0);
   if (up != oldup) {
      if (btn_action_type == TOGGLE_BTN) {
          QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
      }
      else if (btn_action_type == SPRING_BTN) {
         if (!up) {
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
         }
         else {
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);
         }
      }
      SetBgRedrawFlag();
   }
/** NOTE : THIS MAKES A BUTTON THAT IS DOWN SEND A HELD MESSAGE WHEN THE HOVER CHANGES - NOT USEFUL
   else {
      if (!up) {
         QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD);
      }
   }
//*/
   if (hover != oldhover) {
      WidgetBase::SetHoverState(hover);
      if (hover_message_enabled) {
         if (hover) {
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_GAINED_HOVER);
         }
         else {
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_LOST_HOVER);
         }
      }
      if (btn_class == BUTTON_CLASS_HOVER) {
         WidgetBase::SetBgRedrawFlag();
      }
      SetBgRedrawFlag();
   }
   btn_state = (BUTTON_STATE)((hover?2:0) + (up?0:1));
}



void Button::ResetRadii() {
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
            
///            int min = (r.W()< r.H())?(r.W()):(r.H());
///            rad_a = (int)(BTN_ROUNDNESS*min);
//            rad_a = (2*min)/5;
         }
         break;
      case ELLIPSE_BTN : 
         rad_a = r.W()/2;
         rad_b = r.H()/2;
         break;
   }
   SetBgRedrawFlag();
}



void Button::FreeClickArea() {
   if (click_area && delete_area_ptr) {
      delete click_area;
   }
   click_area = 0;
}


/**

   InputGroup input_group;// For holding a key / mouse / jstk button combo to activate the button
   bool pointer_input;

   BUTTON_SHAPE btn_shape;
   BUTTON_ACTION_TYPE btn_action_type;// SPRING_BTN or TOGGLE_BTN
   BUTTON_STATE btn_state;/// BUTTON_STATE
   BUTTON_CLASS btn_class;
///   bool up;
   double dn_time_left;
   EagleFont* text_font;
   std::string text;// gui_textout_ex, gui_strlen from allegro

   /// Button type determines hit detection properties as well
   /// Rectangle button uses dimensions passed in constructor call
   int rad_a;// Primary radius for circles and the horizontal radius for ellipse type buttons,
               // as well as the corner radius for rounded rectangular buttons
   int rad_b;// Secondary vertical radius, only for ellipses

   double spring_duration;
   float hradpercent;// for ROUNDED_BTN type buttons
   float vradpercent;// for ROUNDED_BTN type buttons
   
   /// For tracking state while the button is held
   bool user_activated;
   bool focuskey_activated;
   bool pointer_activated;
   
   AreaBase* click_area;
   bool delete_area_ptr;


   bool hover_message_enabled;

*/
Button::Button() :
      WidgetBase(StringPrintF("Button object at %p" , this)),
      input_group(input_key_press(EAGLE_KEY_NONE)),
      pointer_input(false),
      btn_shape(RECTANGLE_BTN),
      btn_action_type(TOGGLE_BTN),
      btn_state(BUTTON_UP),
      btn_class(BUTTON_CLASS_PLAIN),
      dn_time_left(SPRING_BTN_DURATION),
      text_font(0),
      text(""),
      rad_a(0),
      rad_b(0),
      spring_duration(SPRING_BTN_DURATION),
      hradpercent(0.75),
      vradpercent(0.75),
      user_activated(false),
      focuskey_activated(false),
      pointer_activated(false),
      click_area(0),
      delete_area_ptr(false),
      hover_message_enabled(false)
{
   SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   /// TODO : IMPLEMENT Default Font
   // SetFont(sys->GetDefaultFont());
}



Button::Button(string name) :
      WidgetBase(name),
      input_group(input_key_press(EAGLE_KEY_NONE)),
      pointer_input(false),
      btn_shape(RECTANGLE_BTN),
      btn_action_type(TOGGLE_BTN),
      btn_state(BUTTON_UP),
      btn_class(BUTTON_CLASS_PLAIN),
      dn_time_left(SPRING_BTN_DURATION),
      text_font(0),
      text(""),
      rad_a(0),
      rad_b(0),
      spring_duration(SPRING_BTN_DURATION),
      hradpercent(0.75),
      vradpercent(0.75),
      user_activated(false),
      focuskey_activated(false),
      pointer_activated(false),
      click_area(0),
      delete_area_ptr(false),
      hover_message_enabled(false)
{
   SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   /// TODO : IMPLEMENT Default Font
   // SetFont(sys->GetDefaultFont());
}



Button::Button(string name , BUTTON_SHAPE shape , BUTTON_ACTION_TYPE atype , EagleFont* textfont ,
               string label , const InputGroup& input) :
      WidgetBase(name),
      input_group(input),
      pointer_input(input.PointerInput()),
      btn_shape(RECTANGLE_BTN),
      btn_action_type(atype),
      btn_state(BUTTON_UP),
      btn_class(BUTTON_CLASS_PLAIN),
      dn_time_left(SPRING_BTN_DURATION),
      text_font(0),
      text(label),
      rad_a(0),
      rad_b(0),
      spring_duration(SPRING_BTN_DURATION),
      hradpercent(0.75),
      vradpercent(0.75),
      user_activated(false),
      focuskey_activated(false),
      pointer_activated(false),
      click_area(0),
      delete_area_ptr(false),
      hover_message_enabled(false)
{
   SetButtonType(shape , atype , BUTTON_CLASS_PLAIN);
   SetFont(textfont);
}



Button::~Button() {
   FreeClickArea();
}



int Button::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int Button::PrivateCheckInputs() {

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
         dn_time_left = spring_duration;
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
                  RoundedRectangle rr(InnerArea() , rad_a , rad_b , 1.0f);
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
         default : throw EagleError("Button::PrivateCheckInputs - btn_action_type unknown");break;
      }
      SetButtonState(Flags() & HOVER , up);
      if (WidgetBase::Flags() & ALLOW_CLOSE) {
         retmsg |= DIALOG_CLOSE;
      }
   }
   return retmsg;
   
}



void Button::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DrawButtonShape(win , this , xpos , ypos);
   DrawButtonText(win , this , xpos , ypos);
   ClearRedrawFlag();
}



int Button::PrivateUpdate(double tsec) {
   UINT retmsg = DIALOG_OKAY;
   
   if (btn_action_type == SPRING_BTN) {
      bool up = btn_state%2 == 0;
      bool hover = Flags() & HOVER;
      if (!up && !user_activated && !focuskey_activated && !pointer_activated) {
         dn_time_left -= tsec;
         if (dn_time_left <= 0.0) {
            dn_time_left = spring_duration;
            up = true;
            SetButtonState(hover , up);
         }
      }
   }
   return retmsg;
}


void Button::QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages) {
/**
   WidgetMsg msg(widget_address , widget_topic , messages);
   EagleLog() << "Queueing user message [" << msg << "]. Widget state is currently :" << endl;
   this->DescribeTo(EagleLog());
   EagleLog() << endl;
*/
   WidgetBase::QueueUserMessage(widget_address , widget_topic , messages);
}

/* TODO OLD REMOVE
WidgetMsg Button::CheckInputs(int msx , int msy)// Pass it the mouse position relative to it's drawing target,
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
void Button::DisplayOn(BITMAP* bmp , int x , int y)/// DisplayOn should always draw, and always 
                                                   /// clear the NEEDS_REDRAW flag.
{
   DrawButtonRectangle(bmp , x , y);
   DrawButtonText(bmp , x , y);
   ClearRedrawFlag();
}

*/


/* TODO OLD REMOVE
WidgetMsg Button::Update (double tsec) // logic handling with animation support
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



void Button::SetHoverState (bool state) {
   SetButtonState(state , Up());
}



void Button::SetRedrawFlag() {
   SetBgRedrawFlag();
}



void Button::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetRadii();
   SetBgRedrawFlag();
}



void Button::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetRadii();
   SetBgRedrawFlag();
}



void Button::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetRadii();
   SetBgRedrawFlag();
}



void Button::SetInputGroup(InputGroup ig) {
   input_group = ig;
   pointer_input = input_group.PointerInput();
}



void Button::SetClickArea(AreaBase* btn_click_area , bool delete_when_done) {
   FreeClickArea();
   click_area = btn_click_area;
   delete_area_ptr = delete_when_done;
}



void Button::SetButtonType(BUTTON_SHAPE shape , BUTTON_ACTION_TYPE action_type , BUTTON_CLASS button_class) {
   btn_shape = shape;
   btn_action_type = action_type;
   btn_class = button_class;
   ResetRadii();
   SetBgRedrawFlag();
}



void Button::SetSpringDuration(double duration) {
   if (duration < 0.0) {duration = 0.0;}
   spring_duration = duration;
}



void Button::SetButtonUpState(bool button_up) {
   bool up = btn_state % 2 == 0;
   bool hover = Flags() & HOVER;
   if (up != button_up) {
      up = button_up;
      SetButtonState(hover , up);
      SetBgRedrawFlag();
   }
}



void Button::ToggleButton() {
   bool up = btn_state % 2 == 0;
   bool hover = Flags() & HOVER;
   up = !up;
   SetButtonState(hover , up);
   SetBgRedrawFlag();
}



void Button::SetLabel(string label_text) {
   text = label_text;
   SetBgRedrawFlag();
}




void Button::SetRoundingPercent(float hpercent , float vpercent) {
   if (hpercent < 0.0f) {hpercent = 0.0f;}
   if (hpercent > 1.0f) {hpercent = 1.0f;}
   if (vpercent < 0.0f) {vpercent = 0.0f;}
   if (vpercent > 1.0f) {vpercent = 1.0f;}
   hradpercent = hpercent;
   vradpercent = vpercent;
   ResetRadii();
}




void Button::SetFont(EagleFont* textfont) {

   text_font = textfont;
   SetBgRedrawFlag();

}



void Button::EnableHoverMessage(bool enabled) {
   hover_message_enabled = enabled;
}



InputGroup Button::InputKey() {
   return input_group;
}



bool Button::Up() {
   bool up = (btn_state % 2) == 0;
   return up;
}



std::string Button::Text() {
   return text;
}



EagleFont* Button::Font() {
   return text_font;
}



BUTTON_SHAPE Button::ButtonShape() {
   return btn_shape;
}



BUTTON_STATE Button::ButtonState() {
   return btn_state;
}



int Button::RadiusA() {
   return rad_a;
}



int Button::RadiusB() {
   return rad_b;
}



std::ostream& Button::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("Button object %s at %p :",GetName().c_str(),this) << endl;
   input_group.DescribeTo(os,indent);
   os << indent << "Button shape = [" << GetButtonShapeText(btn_shape) <<
                   "] Button action type = [" << GetButtonActionTypeText(btn_action_type) << "]" << endl;
   os << indent << "Button state = [" << GetButtonStateText(btn_state) << 
                   "] Button class = [" << GetButtonClassText(btn_class) << "]" << endl;
   os << indent << "Text = \"" << text << "\" , Text font = ";
   if (text_font) {
      text_font->DescribeTo(os);
   }
   else {
      os << "NULL" << endl;
   }
   os << indent << "Button radii (a,b) == (" << rad_a << "," << rad_b <<
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




void DrawButtonShape(EagleGraphicsContext* win , Button* btn , int x , int y) {
   if (!btn && !win) {
      throw EagleError("Null win and Null btn passed to DrawButtonShape.\n");
   }
   else if (!btn) {
      throw EagleError("Null btn passed to DrawButtonShape.\n");
   }
   else if (!win) {
      throw EagleError("Null win passed to DrawButtonShape.\n");
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



void DrawButtonText(EagleGraphicsContext* win , Button* btn , int x , int y) {
   if (!btn || !win) {
      throw EagleError("Null win and Null btn passed to DrawButtonText.\n");
   }
   else if (!btn) {
      throw EagleError("Null btn passed to DrawButtonText.\n");
   }
   else if (!win) {
      throw EagleError("Null win passed to DrawButtonText.\n");
   }
   
   EagleFont* text_font = btn->Font();
   string text = btn->Text();
   
   if (text.length()) {
      EAGLE_ASSERT(text_font);
      if (!text_font) {
         throw EagleError(StringPrintF("Button object has text (\"%s\" but no associated font has been set!!!\n",text.c_str()));
      }
      Rectangle r = btn->InnerArea();
      int tx = x + r.X() + r.W()/2;
      int ty = y + r.Y() + r.H()/2;
      
      win->DrawGuiTextString(text_font , text.c_str() , tx , ty , btn->WCols()[TXTCOL] , HALIGN_CENTER , VALIGN_CENTER);
   }
   
}






/// --------------------------------     HoverButton     --------------------------------

/*

HoverButton::HoverButton(std::string name) :
      Button(name)
{}



HoverButton::HoverButton(std::string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont , 
                         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags) :
      Button(name , shape , atype , textfont , label , input , position , wflags)
{}



void HoverButton::DisplayOn(BITMAP* bmp , int x , int y) {
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
         Button::DisplayOn(bmp , x , y);
      }
   }
   else {
      Button::DisplayOn(bmp , x , y);
   }
}


   
void HoverButton::SetHoverState (bool state) {
   WidgetBase::SetHoverState(state);
   SetRedrawFlag();
}
  
*/











/* TODO : OLD REMOVE



#include "Eagle/Gui/Button.hpp"
#include "Eagle/InputHandler.hpp"



const unsigned int TOPIC_BUTTON = NextFreeTopicId();



ButtonBase::ButtonBase() :
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



ButtonBase::ButtonBase(std::string text_str , EagleFont* text_font) :
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



int ButtonBase::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int ButtonBase::CheckInputs() {
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



void ButtonBase::Display(EagleGraphicsContext* win , int xpos , int ypos) {


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



int ButtonBase::Update(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}



/// Highlight Button


void HighlightButton::Display(EagleGraphicsContext* win , int xpos , int ypos) {
   
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





