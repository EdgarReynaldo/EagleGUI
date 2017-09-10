





#ifndef BasicButton_HPP
#define BasicButton_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/InputHandler.hpp"


#include <string>

extern const unsigned int TOPIC_BUTTON_WIDGET;

enum BUTTON_MSGS {
   BUTTON_CLICKED      = 0, /// Sent whenever a spring button is pushed down.
   BUTTON_HELD         = 1, /// Sent whenever a spring button is held down.
   BUTTON_RELEASED     = 2, /// Sent whenever a spring button is released.
   BUTTON_TOGGLED      = 3, /// Sent whenever a toggle button is toggled, poll for the current state.
   BUTTON_GAINED_HOVER = 4, /// Sent whenever a button has the hover, but must be enabled with EnableHoverMessage
   BUTTON_LOST_HOVER   = 5  /// Sent whenever a button has the hover, but must be enabled with EnableHoverMessage
};

REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_HELD);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_GAINED_HOVER);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_LOST_HOVER);




enum BUTTON_SHAPE {
   RECTANGLE_BTN = 0,
   CIRCLE_BTN    = 1,
   ROUNDED_BTN   = 2,
   ELLIPSE_BTN   = 3
};


enum BUTTON_ACTION_TYPE {
   SPRING_BTN = 0,
   TOGGLE_BTN = 1
};


enum BUTTON_CLASS {
   BUTTON_CLASS_PLAIN = 0,
   BUTTON_CLASS_HOVER = 1
};

enum BUTTON_STATE {
   BUTTON_UP = 0,
   BUTTON_DOWN = 1,
   BUTTON_HOVER_UP = 2,
   BUTTON_HOVER_DOWN = 3
};

bool ButtonUp(BUTTON_STATE state);
bool ButtonHover(BUTTON_STATE state);

std::string GetButtonShapeText(BUTTON_SHAPE shape);
std::string GetButtonActionTypeText(BUTTON_ACTION_TYPE type);
std::string GetButtonClassText(BUTTON_CLASS _class);
std::string GetButtonStateText(BUTTON_STATE state);

extern double SPRING_BTN_DURATION;

extern double SPRING_BTN_REPEAT_DELAY;
extern double SPRING_BTN_NUM_REPEAT_PER_SEC;



/// What does a button really need to be a button???



class BasicButton : public WidgetBase {
   
protected :
   
   BUTTON_ACTION_TYPE btn_action_type;
   BUTTON_CLASS btn_class;
   BUTTON_STATE btn_state;
   
   AreaBase* click_area;
   bool delete_area_ptr;
   bool use_default_click_area;
   
   
   
   InputGroup input_group;// For holding a key / mouse / jstk button combo to activate the button
   bool pointer_input;

   double spring_duration;
   double down_time_left;
   
   double repeat_delay;
   double repeat_rate;
   double repeat_elapsed;
   double repeat_previous;
   
   bool user_activated;
   bool focuskey_activated;
   bool pointer_activated;
   bool just_activated;

   bool hover_message_enabled;
   bool held_message_enabled;
   
   void FreeClickArea();

   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int  PrivateUpdate(double tsec);

public :
   
///   BasicButton();
///   BasicButton(std::string name);
   BasicButton(std::string objclass = "BasicButton" , std::string objname = "Nemo");
   
   virtual ~BasicButton();


   virtual void SetButtonType(BUTTON_ACTION_TYPE type);
   
   virtual void SetHoverState (bool state);

   virtual void SetSpringDuration(double duration);
   virtual void SetButtonUpState(bool button_up);
   virtual void ToggleButton();
   virtual void SetClickArea(AreaBase* new_click_area , bool delete_when_done);
   void UseDefaultClickArea(bool use_default);
   
   
   virtual void SetButtonState(bool hover , bool up);
   virtual void SetInputGroup(InputGroup ig);

   virtual bool JustActivated();
   
   virtual void EnableHoverMessage(bool enabled);

   virtual InputGroup InputKey();
   virtual BUTTON_STATE ButtonState();
   
   virtual bool Up();
   virtual bool Hover();
   
   virtual BUTTON_ACTION_TYPE ActionType() {return btn_action_type;}
   
   /// Functions forwarded from WidgetBase
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);/// Pass INT_MAX if you don't care
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);
   
};



#endif // Button_HPP
