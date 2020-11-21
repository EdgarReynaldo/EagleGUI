
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file BasicButton.hpp
 * @brief This file contains a generic, jack of all trades button class, for deriving your own custom buttons from
 * 
 */



#ifndef BasicButton_HPP
#define BasicButton_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/InputHandler.hpp"


#include <string>

extern const unsigned int TOPIC_BUTTON_WIDGET;///< The topic for a button widget

enum BUTTON_MSGS {
   BUTTON_CLICKED      = 0, ///< Sent whenever a spring button is pushed down.
   BUTTON_HELD         = 1, ///< Sent whenever a spring button is held down.
   BUTTON_RELEASED     = 2, ///< Sent whenever a spring button is released.
   BUTTON_TOGGLED      = 3, ///< Sent whenever a toggle button is toggled, poll for the current state.
   BUTTON_GAINED_HOVER = 4, ///v Sent whenever a button has the hover, but must be enabled with EnableHoverMessage
   BUTTON_LOST_HOVER   = 5  ///< Sent whenever a button has the hover, but must be enabled with EnableHoverMessage
};

REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_HELD);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_GAINED_HOVER);
REGISTER_WIDGET_MESSAGE(TOPIC_BUTTON_WIDGET , BUTTON_LOST_HOVER);



/*! \brief BUTTON_SHAPE determines the visual and physical shape of the button */

enum BUTTON_SHAPE {
   RECTANGLE_BTN = 0,///< Generic rectangle button
   CIRCLE_BTN    = 1,///< Circular button
   ROUNDED_BTN   = 2,///< Rounded rectangular button
   ELLIPSE_BTN   = 3 ///< Elliptical button
};

/*! \brief BUTTON_ACTION_TYPE determines how the button behaves when pressed. Spring buttons pop back up, and toggle buttons toggle */

enum BUTTON_ACTION_TYPE {
   SPRING_BTN = 0,///< Spring buttons pop back up after being released
   TOGGLE_BTN = 1 ///< Toggle buttons toggle state on each button press
};


enum BUTTON_CLASS {
   BUTTON_CLASS_PLAIN = 0,
   BUTTON_CLASS_HOVER = 1
};



/** @enum BUTTON_STATE
 *  @brief Simple type to track the button state
 */

enum BUTTON_STATE {
   BUTTON_UP         = 0,///< If the button is up (and no hover)
   BUTTON_DOWN       = 1,///< If the button is down (and no hover)
   BUTTON_HOVER_UP   = 2,///< If the button is up and hovered
   BUTTON_HOVER_DOWN = 3 ///< If the button is down and hovered
};



bool ButtonUp(BUTTON_STATE state);
bool ButtonHover(BUTTON_STATE state);

std::string GetButtonShapeText(BUTTON_SHAPE shape);
std::string GetButtonActionTypeText(BUTTON_ACTION_TYPE type);
std::string GetButtonClassText(BUTTON_CLASS _class);
std::string GetButtonStateText(BUTTON_STATE state);

extern double SPRING_BTN_DURATION;///< How long spring buttons stay down

extern double SPRING_BTN_REPEAT_DELAY;///< How long until the button starts repeating
extern double SPRING_BTN_NUM_REPEAT_PER_SEC;///< Number of repeats in a second


/**! @class BasicButton
 *   @brief The base class for all buttons in Eagle
 */

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
   virtual int PrivateUpdate(double tsec);

   virtual void OnAreaChanged();
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);


public :
   BasicButton(std::string objclass = "BasicButton" , std::string objname = "Nemo");
   
   virtual ~BasicButton();


   virtual void SetButtonType(BUTTON_ACTION_TYPE type);///< Set to SPRING_BTN or TOGGLE_BTN
   
///   virtual void SetHoverState (bool state);

   virtual void SetSpringDuration(double duration);///< Change the down time on a spring button
   virtual void SetButtonUpState(bool button_up);///< Set the button state, eg. for check boxes and such
   virtual void ToggleButton();///< Toggle the button state
   virtual void SetClickArea(AreaBase* new_click_area , bool delete_when_done);///< Set a new click area for this button local to 0,0
   void UseDefaultClickArea(bool use_default);///< Go back to the default click area
   
   
   virtual void SetButtonState(bool hover , bool up , bool notify = true);

   virtual void SetInputGroup(InputGroup ig);///< Set the input group to activate this button

   virtual bool JustActivated();///< Returns true if just activated
   
   virtual void EnableHoverMessage(bool enabled);///< Pass true to enable hover messages

   virtual bool DoIReallyHaveHover(int mx , int my);

   virtual InputGroup InputKey();///< Get the input key
   virtual BUTTON_STATE ButtonState();///< Get the button state
   
   virtual bool Up();///< Returns true if button state is up
   virtual bool Hover();///< Returns true if button hover state is active
   
   virtual BUTTON_ACTION_TYPE ActionType() {return btn_action_type;}///< Returns the current action type of this button
};



#endif // Button_HPP
