



#ifndef ListBox_HPP
#define ListBox_HPP

#include "Eagle/Gui/Layout/MenuLayout.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"



extern const unsigned int TOPIC_LISTBOX;


enum LISTBOX_MESSAGES {
   LISTBOX_SELECTION_MADE = 1
};

REGISTER_WIDGET_MESSAGE(TOPIC_LISTBOX , LISTBOX_SELECTION_MADE);


class ListBox : public ClassicMenuLayout , EagleEventListener {
   

   int choice;
   WidgetBase* wchoice;
public :
   ListBox();
   virtual ~ListBox();
   /// EagleEventListener
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   /// WidgetBase

//   virtual int PrivateHandleEvent(EagleEvent ee);
//   virtual int PrivateCheckInputs();
//   virtual int PrivateUpdate(double dt);
//   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

//   virtual void OnAreaChanged();
//   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
//   virtual void OnColorChanged();

   /// LayoutBase
   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);

   virtual void RemoveWidget(WidgetBase* widget);
   virtual void ClearWidgets();
   
   std::vector<BasicButton*> ButtonsDown();
   int Choice();
   WidgetBase* WChoice();

};

#endif // ListBox_HPP
