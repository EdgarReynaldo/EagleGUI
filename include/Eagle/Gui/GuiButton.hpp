
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
#include "Eagle/Gui/BasicButton.hpp"


/** Basic variety of simple spring or toggle buttons - rectangular, circular, rounded rectangular, and elliptical.
      Support for displaying text on the button included, will probably not look right on the circular buttons due to size.
*/

class AreaBase;

class GuiButton : public BasicButton {

protected :

   BUTTON_SHAPE btn_shape;

   EagleFont* text_font;
   std::string text;

   /** GuiButton type determines hit detection properties as well */
   // Rectangle button uses dimensions passed in constructor call
   int rad_a;// Primary radius for circles and the horizontal radius for ellipse type buttons,
               // as well as the corner radius for rounded rectangular buttons
   int rad_b;// Secondary vertical radius, only for ellipses

   float hradpercent;// for ROUNDED_BTN type buttons
   float vradpercent;// for ROUNDED_BTN type buttons
   

   void ResetRadii();

public :
   /// GuiButton area is determined from the button shape, and the radii are set accordingly
   GuiButton();

   GuiButton(std::string name);

   virtual ~GuiButton() {}

protected :

   virtual int  PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int  PrivateUpdate(double tsec);

public :
   
   virtual void QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages);

   // /* Does not set any redraw flag */ - OLD:Sets redraw flag and (false) sets bg redraw flag

   virtual void SetRedrawFlag();// Shortcut to SetNeedsRedrawState(true)
   
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);


   /// Setters
   void SetButtonType(BUTTON_SHAPE shape , BUTTON_ACTION_TYPE action_type , BUTTON_CLASS button_class);
   
   virtual void SetLabel(std::string label_text);
   
   void SetRoundingPercent(float hpercent , float vpercent);// for ROUNDED_BTN type buttons

   virtual void SetFont(EagleFont* textfont);

   /// Getters
   std::string        Text();
   EagleFont*         Font();
   BUTTON_SHAPE       ButtonShape();
   BUTTON_CLASS       ButtonClass();

   int                RadiusA();
   int                RadiusB();

   virtual std::string GetWidgetClassName();

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};



void DrawGuiButtonShape(EagleGraphicsContext* win , GuiButton* btn , int x , int y);
void DrawGuiButtonText(EagleGraphicsContext* win , GuiButton* btn , int x , int y);




/*
class HoverGuiButton : public GuiButton {
private :

public :
   HoverGuiButton(std::string name);

   HoverGuiButton(std::string name , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype , FONT* textfont , 
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



class GuiButtonBase : public WidgetBase {

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
   
   GuiButtonBase();
   GuiButtonBase(std::string text_str , EagleFont* text_font);

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



class HighlightGuiButton : public GuiButtonBase {

public :
   
   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);
      
   
};

*/





/** TODO : Make button's use signals and slots? **/

///#include "Eagle/MemberFunctors.hpp"

/*

class GuiButton : public WidgetBase {

private :
   Caller2<WidgetBase* , EagleEvent> onclick_caller;
   
public :
   
   template <class T>
   void Bind(T* t , void (T::*tfunc)(WidgetBase* , EagleEvent)) {
      onclick_caller.Add(new MemberFunctor2<T , Widget* , GuiButtonEvent>(t , tfunc));
   }
   
   template <class T>
   void Release(T* t , void (T::*tfunc)(WidgetBase* , EagleEvent) {
      MemberFunctor2<T , Widget* , EagleEvent> memfunc(t,tfunc);
      onclick_caller.Free(&memfunc);
   }
   
};


*/

#endif // EagleGuiGuiButton_HPP

