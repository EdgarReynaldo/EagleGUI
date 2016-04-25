
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

using std::string;


double SPRING_BTN_DURATION = 0.25;
double BTN_ROUNDNESS = 0.4;



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
            int min = (r.W()< r.H())?(r.W()):(r.H());
            rad_a = (int)(BTN_ROUNDNESS*min);
//            rad_a = (2*min)/5;
         }
         break;
      case ELLIPSE_BTN : 
         rad_a = r.W()/2;
         rad_b = r.H()/2;
         break;
   }
}


/** OLD, replaced with global functions

///void Button::DrawButtonRectangle(BITMAP* bmp , int x , int y) {
void Button::DrawButtonRectangle(EagleGraphicsContext* win , int x , int y) {
   Rectangle r = area.InnerArea();
   bool up = 
   r.MoveBy(x,y);
   switch (btn_shape) {
      case RECTANGLE_BTN : 
         up?
         r.DrawGuiRectUp(win , WCols()[FGCOL] , WCols()[SDCOL]):
         r.DrawGuiRectDown(win , WCols()[MGCOL] , WCols()[SDCOL]);
         break;
      case CIRCLE_BTN : 
         up?
         r.DrawGuiCircleUp(win , rad_a , WCols()[FGCOL] , WCols()[SDCOL]):
         r.DrawGuiCircleDown(win , rad_a , WCols()[MGCOL] , WCols()[SDCOL]);
         break;
      case ROUNDED_BTN :
         up?
         r.DrawGuiRoundedUp(win , rad_a , WCols()[FGCOL] , WCols()[SDCOL]):
         r.DrawGuiRoundedDown(win , rad_a , WCols()[MGCOL] , WCols()[SDCOL]);
         break;
      case ELLIPSE_BTN : 
         up?
         r.DrawGuiEllipseUp(win , WCols()[FGCOL] , WCols()[SDCOL]):
         r.DrawGuiEllipseDown(win , WCols()[MGCOL] , WCols()[SDCOL]);
         break;
   }
}



///void Button::DrawButtonText(BITMAP* bmp , int x , int y) {
void Button::DrawButtonText(EagleGraphicsContext* win , int x , int y) {
   if (text_font && text.length()) {
      Rectangle r = area.InnerArea();
      
      int tx = x + r.X() + r.W()/2;
//      int ty = y + r.Y() + (r.H() - text_font->Height())/2 ;
      int ty = y + r.Y() + r.H()/2;
      
      win->DrawGuiTextString(text_font , text.c_str() , tx , ty , WCols()[TXTCOL] , DRAW_TEXT_CENTER , DRAW_TEXT_VCENTER);
   }
}
*/


void Button::FreeClickArea() {
   if (click_area && delete_area_ptr) {
      delete click_area;
   }
   click_area = 0;
}



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
      spring_duration(SPRING_BTN_DURATION),
      roundness(BTN_ROUNDNESS),
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
      spring_duration(SPRING_BTN_DURATION),
      roundness(BTN_ROUNDNESS),
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
      spring_duration(SPRING_BTN_DURATION),
      roundness(BTN_ROUNDNESS),
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
         QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);
         user_activated = false;
         focuskey_activated = false;
         pointer_activated = false;
         SetRedrawFlag();
      } else {
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
               // Lazy hit detection since a rounded rectangle is so close to a rectangle anyway.
               activated = true;
               if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
               break;
            case ELLIPSE_BTN :
               {
                  int xd = msx - (r.X() + r.W()/2);
                  int yd = msy - (r.Y() + r.H()/2);
                  double dist_sq = xd*xd + yd*yd;
                  double angle = atan2(yd,xd);
                  /// TODO: Hmm, distances are just off somehow - hit detection ellipse is slightly larger than drawn ellipse
                  double exd = (double)rad_a*cos(angle);
                  double eyd = (double)rad_b*sin(angle);
//*
                  double ellp_dist = exd*exd + eyd*eyd;
                  if (dist_sq <= ellp_dist) {
                     activated = true;
                     if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
                  }
//*/
///                  EagleLog() << StringPrintF("dist_sq = %f , ellp_dist = %f" , dist_sq , ellp_dist) << std::endl;
               }
               break;
         }
      }
   }
   
   if (activated) {// click or key press has activated the button action
      retmsg |= DIALOG_TAKE_FOCUS;
      retmsg |= DIALOG_INPUT_USED;
      SetBgRedrawFlag();
      bool up = btn_state%2 == 0;
      bool hover = Flags() & HOVER;
      switch (btn_action_type) {
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
      btn_state = (BUTTON_STATE)((hover?2:0) + up?0:1);
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
            btn_state = (BUTTON_STATE)((hover?2:0) + up?0:1);
            SetBgRedrawFlag();
         }
      }
   }
   return retmsg;
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
   bool up = btn_state % 2 == 0;
   bool hover = Flags() & HOVER;
   if (hover != state) {
      btn_state = (BUTTON_STATE)((hover?2:0) + up?0:1);
      if (hover_message_enabled) {
         if (state) {
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_GAINED_HOVER);
         }
         else {
            QueueUserMessage(this , TOPIC_BUTTON_WIDGET , BUTTON_LOST_HOVER);
         }
      }
      WidgetBase::SetHoverState(state);
      if (btn_class == BUTTON_CLASS_HOVER) {
         WidgetBase::SetBgRedrawFlag();
      }
   }
}



void Button::SetRedrawFlag() {
   SetBgRedrawFlag();
}



void Button::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   ResetRadii();
}



void Button::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   ResetRadii();
}



void Button::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   ResetRadii();
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
      btn_state = (BUTTON_STATE)((hover?2:0) + up?0:1);
      SetBgRedrawFlag();
   }
}



void Button::ToggleButton() {
   bool up = btn_state % 2 == 0;
   bool hover = Flags() & HOVER;
   up = !up;
   btn_state = (BUTTON_STATE)((hover?2:0) + up?0:1);
   SetBgRedrawFlag();
}



void Button::SetLabel(string label_text) {
   text = label_text;
   SetBgRedrawFlag();
}



void Button::SetRoundingPercent(double percent) {
   if (percent < 0.0) {percent = 0.0;}
   if (percent > 1.0) {percent = 1.0;}
   roundness = percent;
   int min = (area.W() < area.H())?area.W():area.H();
   min = min/2;
   if (min) {
      min -= 1;
   }
   if (btn_shape == ROUNDED_BTN) {
      rad_a = (int)(roundness*(double)min);
   }
   SetBgRedrawFlag();
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
         r.DrawGuiRoundedUp(win , rad_a , btn->WCols()[FGCOL] , btn->WCols()[SDCOL]):
         r.DrawGuiRoundedDown(win , rad_a , btn->WCols()[MGCOL] , btn->WCols()[SDCOL]);
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
   
   if (text.length() && text_font) {
      Rectangle r = btn->InnerArea();
      int tx = x + r.X() + r.W()/2;
      int ty = y + r.Y() + r.H()/2;
      
      win->DrawGuiTextString(text_font , text.c_str() , tx , ty , btn->WCols()[TXTCOL] , DRAW_TEXT_CENTER , DRAW_TEXT_VCENTER);
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





