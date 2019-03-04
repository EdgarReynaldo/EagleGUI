


#include "Eagle/Area.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"

#include <sstream>
using namespace std;


const unsigned int TOPIC_BUTTON_WIDGET = NextFreeTopicId();

REGISTERED_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
REGISTERED_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_HELD);
REGISTERED_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);
REGISTERED_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
REGISTERED_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_GAINED_HOVER);
REGISTERED_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_LOST_HOVER);



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


/// Lazy TODO why do these use stringstreams? :/
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



double SPRING_BTN_DURATION = 0.25;

double SPRING_BTN_REPEAT_DELAY = 0.5;
double SPRING_BTN_NUM_REPEAT_PER_SEC = 16;




/// ------------------------------     BasicButton     --------------------------------------




void BasicButton::FreeClickArea() {
   if (click_area) {
      if (delete_area_ptr) {
         delete click_area;
      }
      click_area = 0;
   }
}



int BasicButton::PrivateCheckInputs() {

   unsigned int retmsg = DIALOG_OKAY;
   bool activated = false;
   bool released = false;

   int msx = mouse_x;
   int msy = mouse_y;
   WidgetHandler* handler = 0;
   
   if (wparent) {
      handler = dynamic_cast<WidgetHandler*>(wparent);
   }
   if (handler) {
      msx = handler->GetMouseX();
      msy = handler->GetMouseY();
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
         SetButtonState(Flags().FlagOn(HOVER) , true);
         user_activated = false;
         focuskey_activated = false;
         pointer_activated = false;
      } else {/// HELD
         down_time_left = spring_duration;
///         RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD));/// Don't send HELD message - it just spams everything
      }
      return DIALOG_OKAY;
   }


   if (input_group) {
      EagleInfo() << "input_group active" << std::endl;
      activated = true;
      if (btn_action_type == SPRING_BTN) {user_activated = true;}
   
   } else if (Flags().FlagOn(HASFOCUS) && (input_key_press(EAGLE_KEY_SPACE) || input_key_press(EAGLE_KEY_ENTER))) {
      
      activated = true;
      if (btn_action_type == SPRING_BTN) {focuskey_activated = true;}
      
   } else if (input_mouse_press(LMB) && InnerArea().Contains(msx,msy)) {
      if (click_area) {
         if (click_area->Contains(msx - InnerArea().X() , msy - InnerArea().Y())) {
            activated = true;
            EagleInfo() <<  "Click area activated." << std::endl;
            if (btn_action_type == SPRING_BTN) {pointer_activated = true;}
         }
      }
   }
   just_activated = false;
   if (activated) {// click or key press has activated the button action
      just_activated = true;
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
         default : throw EagleException("BasicButton::PrivateCheckInputs - btn_action_type unknown");break;
      }
      SetButtonState(Flags().FlagOn(HOVER) , up);
      if (Flags().FlagOn(ALLOW_CLOSE)) {
         retmsg |= DIALOG_CLOSE;
      }
   }
   return retmsg;
   
}



void BasicButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (click_area) {
      int xoffset = InnerArea().X() + xpos;
      int yoffset = InnerArea().Y() + ypos;
      click_area->MoveBy(xoffset , yoffset);
      WidgetColorset wc = WidgetColors();
      if (Up()) {
         click_area->Fill(win , wc[MGCOL]);
         click_area->Draw(win , wc[FGCOL]);
      }
      else {
         click_area->Fill(win , wc[BGCOL]);
         click_area->Draw(win , wc[MGCOL]);
      }
      click_area->MoveBy(-xoffset , -yoffset);
   }
}



int BasicButton::PrivateUpdate(double tsec) {
   if (btn_action_type == SPRING_BTN) {
      bool up = (btn_state%2) == 0;
      bool hover = Flags().FlagOn(HOVER);
      
      if (!up) {
         repeat_elapsed += tsec;
         if (repeat_elapsed >= repeat_delay) {
            if (repeat_previous < repeat_delay) {
               repeat_previous = repeat_delay;
            }
            double dt = repeat_elapsed - repeat_previous;
            int num_repeat = dt/repeat_rate;
            for (int i = 0 ; i < num_repeat ; ++i) {
               RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD));
            }
            repeat_previous += (double)num_repeat*repeat_rate;
            EAGLE_ASSERT(repeat_previous <= repeat_elapsed);
         }
      }
      
      if (!up && !user_activated && !focuskey_activated && !pointer_activated) {
         down_time_left -= tsec;
         if (down_time_left <= 0.0) {
            down_time_left = spring_duration;
            up = true;
            SetButtonState(hover , up);
         }
      }
   }
   return DIALOG_OKAY;
}



void BasicButton::OnAreaChanged() {
   UseDefaultClickArea(use_default_click_area);
}



BasicButton::BasicButton(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      btn_action_type(SPRING_BTN),
      btn_class(BUTTON_CLASS_PLAIN),
      btn_state(BUTTON_UP),
      click_area(0),
      delete_area_ptr(false),
      use_default_click_area(true),
      input_group(),
      pointer_input(false),
      spring_duration(SPRING_BTN_DURATION),
      down_time_left(0.0),
      repeat_delay(SPRING_BTN_REPEAT_DELAY),
      repeat_rate(1.0/SPRING_BTN_NUM_REPEAT_PER_SEC),
      repeat_elapsed(0.0),
      repeat_previous(0.0),
      user_activated(false),
      focuskey_activated(false),
      pointer_activated(false),
      just_activated(false),
      hover_message_enabled(false),
      held_message_enabled(false)
{
   UseDefaultClickArea(true);
}



BasicButton::~BasicButton() {
   FreeClickArea();
}




void BasicButton::SetButtonType(BUTTON_ACTION_TYPE type) {
   btn_action_type = type;
   bool hover = Flags().FlagOn(HOVER);
   bool up = true;
   btn_state = (BUTTON_STATE)((hover?2:0) + (up?0:1));
}



void BasicButton::SetHoverState (bool state) {
   SetButtonState(state , Up());
}



void BasicButton::SetSpringDuration(double duration) {
   if (duration < 0.0) {duration = 0.0;}
   spring_duration = duration;
}



void BasicButton::SetButtonUpState(bool button_up) {
   bool up = btn_state % 2 == 0;
   bool hover = (unsigned int)Flags() & HOVER;
   if (up != button_up) {
      up = button_up;
      SetButtonState(hover , up);
      SetBgRedrawFlag();
   }
}



void BasicButton::ToggleButton() {
   bool up = btn_state % 2 == 0;
   bool hover = (unsigned int)Flags() & HOVER;
   up = !up;
   SetButtonState(hover , up);
   SetBgRedrawFlag();
}


void BasicButton::SetClickArea(AreaBase* new_click_area , bool delete_when_done) {
   FreeClickArea();
   click_area = new_click_area;
   /// To clone or not to clone
///   click_area = new_click_area->Clone();
   delete_area_ptr = delete_when_done;
}



void BasicButton::UseDefaultClickArea(bool use_default) {
   use_default_click_area = use_default;
   if (use_default_click_area) {
      SetClickArea(new Rectangle(0 , 0 , InnerArea().W() , InnerArea().H()) , true);
///      SetClickArea(new Rectangle(area.MLeft() , area.MTop() , InnerArea().W() , InnerArea().H()) , true);
   }
}



void BasicButton::SetButtonState(bool hover , bool up) {
   bool oldhover = (unsigned int)Flags() & HOVER;
   bool oldup = (btn_state % 2 == 0);
   if (up != oldup) {
      if (btn_action_type == TOGGLE_BTN) {
          RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED));
      }
      else if (btn_action_type == SPRING_BTN) {
         if (!up) {
            RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED));
            repeat_elapsed = repeat_previous = 0.0;
         }
         else {
            RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_RELEASED));
         }
      }
      SetBgRedrawFlag();
   }
/** NOTE : THIS MAKES A BUTTON THAT IS DOWN SEND A HELD MESSAGE WHEN THE HOVER CHANGES - NOT USEFUL
   else {
      if (!up) {
         RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD));
      }
   }
//*/
   if (hover != oldhover) {
      WidgetBase::SetHoverState(hover);
      if (hover_message_enabled) {
         if (hover) {
            RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_GAINED_HOVER));
         }
         else {
            RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_LOST_HOVER));
         }
      }
      if (btn_class == BUTTON_CLASS_HOVER) {
         WidgetBase::SetBgRedrawFlag();
      }
      SetBgRedrawFlag();
   }
   btn_state = (BUTTON_STATE)((hover?2:0) + (up?0:1));
}




void BasicButton::SetInputGroup(InputGroup ig) {
   input_group = ig;
   pointer_input = input_group.PointerInput();
}




bool BasicButton::JustActivated() {
   return just_activated;
}



void BasicButton::EnableHoverMessage(bool enabled) {
   hover_message_enabled = enabled;
}



InputGroup BasicButton::InputKey() {
   return input_group;
}



BUTTON_STATE BasicButton::ButtonState() {
   return btn_state;
}



bool BasicButton::Up() {
   return ButtonUp(btn_state);
}



bool BasicButton::Hover() {
   return ButtonHover(btn_state);
}


