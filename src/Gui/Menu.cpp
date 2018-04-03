



#include "Eagle/Gui/Menu.hpp"
#include "Eagle/Gui/Button/CheckBox.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/Font.hpp"


const int TOPIC_MENU_MESSAGE = NextFreeTopicId();



REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_OPENED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_CLOSED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);







void DrawMenuArrow(EagleGraphicsContext* win , Rectangle r , const WidgetColorset& wc) {
   Pos2D ul(r.X() + 0.3*r.W() , r.Y() + 0.2*r.H());
   Pos2D rt(r.X() + 0.7*r.W() , r.Y() + 0.5*r.H());
   Pos2D bl(r.X() + 0.3*r.W() , r.Y() + 0.8*r.H());
   Triangle t(ul.X() , ul.Y() , rt.X() , rt.Y() , bl.X() , bl.Y());
   t.Fill(win , wc[SDCOL]);
   t.Draw(win , 2.0 , wc[FGCOL]);
}



/// ----------------------           MenuItemBase      --------------------------



MenuItemBase::MenuItemBase(std::string classname , std::string objname) :
      WidgetBase(classname , objname),
      sub_menu(0)
{}



MenuItemBase::MenuItemBase(std::string objname) :
      WidgetBase("MenuItemBase" , objname),
      sub_menu(0)
{}



MenuItemBase::~MenuItemBase() {}



bool MenuItemBase::SubMenuOpen() {
   if (sub_menu) {
      return sub_menu->IsOpen();
   }
   return false;
}



void MenuItemBase::OpenSubMenu() {
   if (sub_menu) {
      sub_menu->Open();
   }
}



void MenuItemBase::CloseSubMenu() {
   if (sub_menu) {
      sub_menu->Close();
   }
}



/// ---------------------    ExampleMenuItem     --------------------------





int ExampleMenuItem::PrivateHandleEvent(EagleEvent e) {
   EagleEvent esub = e;
   if (IsMouseEvent(e)) {
      esub.mouse.x -= InnerArea().X();
      esub.mouse.y -= InnerArea().Y();
   }
   int ret = our_button.HandleEvent(esub);
   return ret;
}


int ExampleMenuItem::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void ExampleMenuItem::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   xpos += InnerArea().X();
   ypos += InnerArea().Y();
   
   win->DrawFilledRectangle(OuterArea() , WCols()[(Flags() & HOVER)?HVRCOL:BGCOL]);
   
   our_title_text.Display(win , xpos , ypos);
   our_key_text.Display(win , xpos , ypos);
   
   if (have_state) {
      Rectangle cb = LayoutArea(InnerArea() , LayoutRectangle(0.0 , 0.0 , 0.1 , 1.0));
      ///DefaultCBDrawFunc(EagleGraphicsContext* win , Rectangle area , int xpos , int ypos , const WidgetColorset& wc , bool up , bool hover);
      DefaultCBDrawFunc(win , cb , 0 , 0 , WCols() , our_button.Up() , our_button.Hover());
   }
   else if (sub_menu) {
      Rectangle arrow = LayoutArea(InnerArea() , LayoutRectangle(0.9 , 0.0 , 0.1 , 1.0));
      DrawMenuArrow(win , arrow , WCols());
      if (sub_menu->IsOpen()) {
         /** Let the GUI handle displaying the other menus */
///         sub_menu->Display(win , OuterArea().X() + xpos , OuterArea().Y() + ypos);
      }
   }
}



int ExampleMenuItem::PrivateUpdate(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}



ExampleMenuItem::ExampleMenuItem(std::string objname) :
      MenuItemBase("ExampleMenuItem" , objname),
      our_layout(objname + "::our_layout"),
      our_button(objname + "::our_button"),
      our_title_text(objname + "::our_title_text"),
      our_key_text(objname + "::our_key_text"),
      have_state(false)
{
   our_layout.Resize(2);
   our_title_text.Realign(HALIGN_LEFT , VALIGN_CENTER , 2 , 2);
   our_key_text.Realign(HALIGN_RIGHT , VALIGN_CENTER , 2 , 2);
   our_layout.PlaceWidget(&our_title_text , 0 , LayoutRectangle(0.1 , 0.0 , 0.8 , 1.0));
   our_layout.PlaceWidget(&our_key_text , 0 , LayoutRectangle(0.1 , 0.0 , 0.8 , 1.0));
}



void ExampleMenuItem::QueueUserMessage(const WidgetMsg& wmsg) {
   const WidgetMsg click(&our_button , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg toggle(&our_button , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
   if (wmsg == click) {
      WidgetBase::QueueUserMessage(this , TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
   }
   else if (wmsg == toggle) {
      WidgetBase::QueueUserMessage(this , TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);
   }
   return;
}



void ExampleMenuItem::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos , ypos , width , height , notify_layout);
   our_layout.SetWidgetArea(0 , 0 , InnerArea().W() , InnerArea().H() , false);/// sets the area of our text
   our_button.SetClickArea(new Rectangle(InnerArea()) , true);
   if (sub_menu) {
      sub_menu->SetWidgetPos(OuterArea().BRX() , OuterArea().Y());
   }
}



void ExampleMenuItem::MakeStateButton(bool has_state , bool active) {
   have_state = has_state;
   our_button.SetButtonType(have_state?TOGGLE_BTN:SPRING_BTN);
   if (have_state) {
      our_button.SetButtonState(our_button.Hover() , !active);
      SetSubMenu(0);
   }
}



void ExampleMenuItem::SetSubMenu(Menu* m) {
   if (sub_menu) {
      CloseSubMenu();
   }
   sub_menu = m;
   if (sub_menu) {
      MakeStateButton(false , false);
      sub_menu->SetParent(this);
   }
}



void ExampleMenuItem::SetInfo(EagleFont* f , std::string title , std::string key_str , InputGroup input_key) {
   our_title_text.SetText(title , f);
   our_key_text.SetText(key_str , f);
   our_button.SetInputGroup(input_key);
}



bool ExampleMenuItem::JustActivated() {
   return our_button.JustActivated();
}



bool ExampleMenuItem::HasState() {
   return have_state;
}



ExampleMenuItem* CreateExampleMenuItem(EagleFont* f , std::string title , std::string key_str , InputGroup input , Menu* submenu) {
   ExampleMenuItem* m = new ExampleMenuItem(title + " Menu Item");
   m->SetInfo(f,title,key_str,input);
   m->SetSubMenu(submenu);
   return m;
}



ExampleMenuItem* CreateExampleMenuToggleItem(EagleFont* f , std::string title , std::string key_str , InputGroup input , bool active) {
   ExampleMenuItem* m = new ExampleMenuItem(title + " Menu Item");
   m->SetInfo(f,title,key_str,input);
   m->MakeStateButton(true , active);
   return m;
}



/// ---------------------     Menu     -----------------------




int Menu::MenuIndex(WidgetBase* w) {
   std::vector<WidgetBase*> c = our_layout->WChildren();
   for (int i = 0 ; i < (int)c.size() ; ++i) {
      if (w == c[i]) {
         return i;
      }
   }
   return -1;
}



void Menu::OpenSubMenu(MenuItemBase* menu_to_open) {
   EAGLE_ASSERT(menu_to_open);
   if (menu_to_open != active_menu_item) {
      CloseSubMenu();
   }
   menu_to_open->OpenSubMenu();
   active_menu_item = menu_to_open;
   active_menu_index = MenuIndex(active_menu_item);
}



void Menu::CloseSubMenu() {
   if (active_menu_item) {
      active_menu_item->CloseSubMenu();
   }
   active_menu_item = 0;
   active_menu_index = -1;
}



int Menu::PrivateHandleEvent(EagleEvent e) {
   
   int ret = DIALOG_OKAY;
   
   MenuItemBase* active_item = 0;
   
   std::vector<WidgetBase*> children = our_layout->WChildren();

   if (Flags() & HASFOCUS) {


      /// Check hover 
      if (e.type == EAGLE_EVENT_MOUSE_AXES) {
         if (InnerArea().Contains(e.mouse.x , e.mouse.y)) {
            for (unsigned int i = 0 ; i < children.size() ; ++i) {
               Rectangle r = children[i]->InnerArea();
               r.MoveBy(InnerArea().X() , InnerArea().Y());
               if (r.Contains(e.mouse.x , e.mouse.y)) {
                  active_item = dynamic_cast<MenuItemBase*>(children[i]);
               }
            }
         }
      }
      
      /// Check keyboard navigation
      if (e.type == EAGLE_EVENT_KEY_CHAR) {
         int NCHLD = (int)our_layout->WChildren().size();
         if (e.keyboard.keycode == EAGLE_KEY_LEFT || e.keyboard.keycode == EAGLE_KEY_UP) {
            int menu_index = (active_menu_index + NCHLD - 1)%NCHLD;
            active_item = dynamic_cast<MenuItemBase*>(children[menu_index]);
         }
         else if (e.keyboard.keycode == EAGLE_KEY_RIGHT || e.keyboard.keycode == EAGLE_KEY_DOWN) {
            int menu_index = (active_menu_index + 1)%NCHLD;
            active_item = dynamic_cast<MenuItemBase*>(children[menu_index]);
         }
      }
      if (active_item && active_item != active_menu_item) {
         OpenSubMenu(active_item);
         ret |= DIALOG_INPUT_USED;
      }
   }

   for (int i = 0 ; i < (int)children.size() ; ++i) {
      WidgetBase* w = children[i];
      w->HandleEvent(e);
   }

   return ret;
}



int Menu::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void Menu::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   xpos += InnerArea().X();
   ypos += InnerArea().Y();
   std::vector<WidgetBase*> children = our_layout->WChildren();
   for (int i = 0 ; i < (int)children.size() ; ++i) {
      WidgetBase* w = children[i];
      w->Display(win , xpos , ypos);
   }
}



int Menu::PrivateUpdate(double tsec) {
   std::vector<WidgetBase*> children = our_layout->WChildren();
   for (int i = 0 ; i < (int)children.size() ; ++i) {
      WidgetBase* w = children[i];
      w->Update(tsec);
   }
   return DIALOG_OKAY;
}



void Menu::UseLayout(Layout* l) {
   EAGLE_ASSERT(l);
   CloseSubMenu();
   std::vector<WidgetBase*> children;
   if (our_layout && our_layout != l) {
      children = our_layout->WChildren();
   }
   our_layout = l;
   our_layout->SetWChildren(children);
   active_menu_item = 0;
   active_menu_index = -1;
}




void Menu::PlaceWidget(WidgetBase* widget , int slot) {
   EAGLE_ASSERT(our_layout);

   our_layout->PlaceWidget(widget , slot);

   widget->SetParent(this);
}



int Menu::AddWidget(WidgetBase* widget) {
   EAGLE_ASSERT(our_layout);

   widget->SetParent(this);

   return our_layout->AddWidget(widget);
}



void Menu::Open() {
   SetVisibilityState(true);
   SetEnabledState(true);
   we_are_open = true;
}



void Menu::Close() {
   CloseSubMenu();
   SetVisibilityState(false);
   SetEnabledState(false);
   we_are_open = false;
}



WidgetMsg Menu::ItemActivationMsg(int item_num) {
   if (item_num < 0 || item_num >= (int)our_layout->WChildren().size()) {
      throw EagleException(StringPrintF("Menu::ItemActivationMsg - Item %d out of range [0,%d)" , item_num , (int)our_layout->WChildren().size()));
   }
   MenuItemBase* i = dynamic_cast<MenuItemBase*>(our_layout->WChildren()[item_num]);
   EAGLE_ASSERT(!i->HasState());
   return WidgetMsg(i , TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
}



WidgetMsg Menu::ItemToggleMsg    (int item_num) {
   if (item_num < 0 || item_num >= (int)our_layout->WChildren().size()) {
      throw EagleException(StringPrintF("Menu::ItemActivationMsg - Item %d out of range [0,%d)" , item_num , (int)our_layout->WChildren().size()));
   }
   MenuItemBase* i = dynamic_cast<MenuItemBase*>(our_layout->WChildren()[item_num]);
   EAGLE_ASSERT(i->HasState());
   return WidgetMsg(i , TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);
}



