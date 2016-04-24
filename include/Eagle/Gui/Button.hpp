
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
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleGuiButton_HPP
#define EagleGuiButton_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Font.hpp"

#include <string>

#include "Eagle/InputHandler.hpp"



extern const unsigned int TOPIC_BUTTON_WIDGET;

enum BUTTON_MSGS {
   BUTTON_CLICKED      = 0, // Sent whenever a spring button is pushed down.
   BUTTON_HELD         = 1, // Sent whenever a spring button is held down.
   BUTTON_RELEASED     = 2, // Sent whenever a spring button is released.
   BUTTON_TOGGLED      = 3, // Sent whenever a toggle button is toggled, poll for the current state.
   BUTTON_GAINED_HOVER = 4, // Sent whenever a button has the hover, but must be enabled with EnableHoverMessage
   BUTTON_LOST_HOVER   = 5  // Sent whenever a button has the hover, but must be enabled with EnableHoverMessage
};

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

extern double SPRING_BTN_DURATION;// Set before instantiating a new Button or use Button::SetSpringDuration
//extern double BTN_ROUNDNESS = 0.4;

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

/** Basic variety of simple spring or toggle buttons - rectangular, circular, rounded rectangular, and elliptical.
      Support for displaying text on the button included, will probably not look right on the circular buttons due to size.
*/

class AreaBase;

class Button : public WidgetBase {

protected :

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

   /** Button type determines hit detection properties as well */
   // Rectangle button uses dimensions passed in constructor call
   int rad_a;// Primary radius for circles and the horizontal radius for ellipse type buttons,
               // as well as the corner radius for rounded rectangular buttons
   int rad_b;// Secondary vertical radius, only for ellipses

   double spring_duration;
   double roundness;// for ROUNDED_BTN type buttons
   
   /// For tracking state while the button is held
   bool user_activated;
   bool focuskey_activated;
   bool pointer_activated;
   
   AreaBase* click_area;
   bool delete_area_ptr;


   bool hover_message_enabled;



   void ResetRadii();
   
///   void DrawButtonRectangle(BITMAP* bmp , int x , int y);
///   void DrawButtonText(BITMAP* bmp , int x , int y);
///   void DrawButtonRectangle(EagleGraphicsContext* win , int x , int y);
///   void DrawButtonText(EagleGraphicsContext* win , int x , int y);

   void FreeClickArea();



public :
   /// Button area is determined from the button shape, and the radii are set accordingly
   Button();

   Button(std::string name);

   Button(std::string name , BUTTON_SHAPE shape , BUTTON_ACTION_TYPE atype , EagleFont* textfont , 
         std::string label , const InputGroup& input);

   virtual ~Button();

protected :
   virtual int  PrivateHandleEvent(EagleEvent e);
   virtual int  PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int  PrivateUpdate(double tsec);

///   virtual WidgetMsg Update (double tsec);
///   virtual WidgetMsg CheckInputs(int msx , int msy);
///   virtual void DisplayOn(BITMAP* bmp , int x , int y);

public :
   
   virtual void SetHoverState (bool state);
   // /* Does not set any redraw flag */ - OLD:Sets redraw flag and (false) sets bg redraw flag

   virtual void SetRedrawFlag();// Shortcut to SetNeedsRedrawState(true)
   
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);


   /// Setters
   void SetInputGroup(InputGroup ig);
   void SetClickArea(AreaBase* btn_click_area , bool delete_when_done);// Relative to widget, NOT absolute
   void SetButtonType(BUTTON_SHAPE shape , BUTTON_ACTION_TYPE action_type , BUTTON_CLASS button_class);
   void SetSpringDuration(double duration);
   void SetButtonUpState(bool button_up);
   void ToggleButton();
   
   virtual void SetLabel(std::string label_text);
   
   void SetRoundingPercent(double percent);// for ROUNDED_BTN type buttons

   virtual void SetFont(EagleFont* textfont);
   
   void EnableHoverMessage(bool enabled);

   /// Getters
   InputGroup   InputKey();
   bool         Up();
   std::string  Text();
   EagleFont*   Font();
   BUTTON_SHAPE ButtonShape();
   BUTTON_STATE ButtonState();
   int          RadiusA();
   int          RadiusB();
};



void DrawButtonShape(EagleGraphicsContext* win , Button* btn , int x , int y);
void DrawButtonText(EagleGraphicsContext* win , Button* btn , int x , int y);




/*
class HoverButton : public Button {
private :

public :
   HoverButton(std::string name);

   HoverButton(std::string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont , 
         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetHoverState (bool state);
   
};
*/





/*
extern const unsigned int TOPIC_BUTTON;



enum BUTTON_MSGS {
   BUTTON_CLICKED = 0,
   BUTTON_RELEASED = 1,
   BUTTON_HOVER = 2
};



class ButtonBase : public WidgetBase {

protected :
   bool down;
   bool hover;
   bool selected;
   
   EagleImage* upimage;
   EagleImage* downimage;
   EagleImage* hoverimage;
   EagleImage* selectedimage;
   
   std::string text;
   EagleFont* font;
   
public :   
   
   ButtonBase();
   ButtonBase(std::string text_str , EagleFont* text_font);

public :
   
   EagleColor outer_color;
   EagleColor down_color;
   EagleColor up_color;
   EagleColor hover_color;
   EagleColor selected_color;
   EagleColor text_color;
   EagleColor text_color2;

   
   
/// WIDGETBASE   
protected :
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int CheckInputs();
public :
   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int Update(double tsec);
   
   
   void SetSelected(bool select_status) {selected = select_status;SetRedrawFlag();}
   

};



class HighlightButton : public ButtonBase {

public :
   
   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);
      
   
};

*/





/** TODO : Make button's use signals and slots? **/

///#include "Eagle/MemberFunctors.hpp"

/*

class Button : public WidgetBase {

private :
   Caller2<WidgetBase* , EagleEvent> onclick_caller;
   
public :
   
   template <class T>
   void Bind(T* t , void (T::*tfunc)(WidgetBase* , EagleEvent)) {
      onclick_caller.Add(new MemberFunctor2<T , Widget* , ButtonEvent>(t , tfunc));
   }
   
   template <class T>
   void Release(T* t , void (T::*tfunc)(WidgetBase* , EagleEvent) {
      MemberFunctor2<T , Widget* , EagleEvent> memfunc(t,tfunc);
      onclick_caller.Free(&memfunc);
   }
   
};


*/

#endif // EagleGuiButton_HPP

