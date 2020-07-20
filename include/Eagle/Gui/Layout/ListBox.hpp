



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

public :
   ListBox();
   virtual ~ListBox();
   /// EagleEventListener
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   /// WidgetBase
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);

   /// LayoutBase
   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);

   virtual void RemoveWidget(WidgetBase* widget);
   virtual void ClearWidgets();
   
   /// Members
   void SetChoice(int c);
   
   std::vector<BasicButton*> ButtonsDown();
   int Choice();
};

#endif // ListBox_HPP
