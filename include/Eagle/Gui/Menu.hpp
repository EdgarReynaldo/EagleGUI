



#ifndef Menu_HPP
#define Menu_HPP

#include "Eagle/Gui/WidgetBase.hpp"

#include "Eagle/Gui/Button/BasicButton.hpp"

#include "Eagle/Gui/Text/BasicText.hpp"



extern const int TOPIC_MENU_MESSAGE;

//REGISTER_WIDGET_MESSAGE(topic , msg)

enum MENU_MESSAGES {
   
   MENU_OPENED = 0,
   MENU_CLOSED = 1,
   MENU_ITEM_ACTIVATED = 2
   
};


REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_OPENED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_CLOSED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);



class Menu;



class MenuItem {
   
   BasicButton* menu_button;
   BasicText* menu_text;
   
   Menu* sub_menu;
   
   bool menu_is_open;
   
   
public :
   
   virtual bool IsOpen()=0;
   virtual bool IsActivated()=0;
   
};





#endif // Menu_HPP

