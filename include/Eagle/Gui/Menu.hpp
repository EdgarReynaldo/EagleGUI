



#ifndef Menu_HPP
#define Menu_HPP

#include "Eagle/Gui/WidgetBase.hpp"

#include "Eagle/Gui/Button/BasicButton.hpp"

#include "Eagle/Gui/Text/BasicText.hpp"

#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/Layout/RelativeLayout.hpp"


extern const int TOPIC_MENU_MESSAGE;

//REGISTER_WIDGET_MESSAGE(topic , msg)

enum MENU_MESSAGES {
   
   MENU_OPENED = 0,
   MENU_CLOSED = 1,
   MENU_ITEM_ACTIVATED = 2,
   MENU_ITEM_TOGGLED = 3
   
};


REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_OPENED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_CLOSED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);



void DrawMenuArrow(EagleGraphicsContext* win , Rectangle r , const WidgetColorset& wc);



class Menu;

class MenuItemBase : public WidgetBase {
   
protected :
   Menu* sub_menu;

   /// WidgetBase, these need to be overloaded

   MenuItemBase(std::string classname , std::string objname = "Nemo");
public :
   MenuItemBase(std::string objname = "Nemo");

   virtual ~MenuItemBase();
   
   bool SubMenuOpen();
   virtual bool JustActivated()=0;
   
   void OpenSubMenu();
   void CloseSubMenu();

   virtual bool HasState()=0;
};



class ExampleMenuItem : public MenuItemBase {
   
protected :
   RelativeLayout our_layout;
   BasicButton our_button;
   BasicText our_title_text;
   BasicText our_key_text;
   
   bool have_state;/// If we are a button with state like up and down. If yes we display a check box, if no we display normally

   /// WidgetBase, these need to be overloaded
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

public :
   
   ExampleMenuItem(std::string objname = "Nemo");
   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);





   void MakeStateButton(bool has_state , bool active);
   
   void SetSubMenu(Menu* m);
   
   void SetInfo(EagleFont* f , std::string title , std::string key_str = "" , InputGroup input_key = input_key_press(EAGLE_KEY_NONE));

   virtual bool JustActivated();
   virtual bool HasState();
};

ExampleMenuItem* CreateExampleMenuItem(EagleFont* f , std::string title , std::string key_str , InputGroup input , Menu* submenu = 0);
ExampleMenuItem* CreateExampleMenuToggleItem(EagleFont* f , std::string title , std::string key_str , InputGroup input , bool active);


/**
Menu* m = new Menu(new MenuLayout);
m->Resize(10);
vector<MenuItem*> item_array1 = {
   new ExampleMenuItem(new GuiButton("mb1" , new BasicText(font , "Menu button 1"))),
   new ExampleMenuItem(new BasicButton , new BasicText),
   new ExampleMenuItem(new BasicButton , new BasicText),
   new ExampleMenuItem(new BasicButton , new BasicText),
   
   
   new ExampleMenuItem(toggle , input_key_press(EAGLE_KEY_A) , "Menu A" , "A" , submenu),
   
   
};
m->SetChildrenItems(item_array1);

while (gui.HasMessages()) {
   WidgetMsg msg = gui.TakeNextMessage();
   if (msg
}

*/

class Menu : public WidgetBase {
   
protected :
   
   Layout* our_layout;
   
   std::vector<MenuItemBase*> items;
   MenuItemBase* active_menu_item;

   int active_menu_index;
   
   bool we_are_open;
   


   int MenuIndex(WidgetBase* w);

   void OpenSubMenu(MenuItemBase* menu_to_open);
   void CloseSubMenu();
   
   /// WidgetBase
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);
   
public :   
   
   /// Layout
   void UseLayout(Layout* layout);
   
   /// Widget may be null for PlaceWidget
   /// Both replace the widget (addwidget replaces a null widget) and call RepositionChild
   virtual void PlaceWidget(WidgetBase* widget , int slot);
   virtual int AddWidget(WidgetBase* widget);/// Adds the widget to the next free slot or creates one if necessary, returns slot used
   
   /// Menu
   virtual void Open();
   virtual void Close();

   bool IsOpen() {return we_are_open;}
   
   /// To check for messages, compare against what these return
   WidgetMsg ItemActivationMsg(int item_num);
   WidgetMsg ItemToggleMsg(int item_num);
   
};





#endif // Menu_HPP

