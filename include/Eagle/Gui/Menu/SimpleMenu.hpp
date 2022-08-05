



#ifndef SimpleMenu_HPP
#define SimpleMenu_HPP


#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/InputHandler.hpp"

class EagleImage;
class EagleFont;

#include <string>
#include <memory>


class SIMPLE_MENU_ITEM {
public :
   BUTTON_ACTION_TYPE   batype;
   bool              down;
   EagleImage*       image_up;
   EagleImage*       image_dn;
   EagleFont*        text_font;
   std::string       description;
   InputGroup        action_key;
   std::string       key_text;
   SIMPLE_MENU_ITEM* sub_menu;
};


class ClassicMenuBar;
class ClassicMenu;

std::shared_ptr<ClassicMenuBar> MakeMenuBar(SIMPLE_MENU_ITEM* items , int nitems);
std::shared_ptr<ClassicMenu> MakeMenu(SIMPLE_MENU_ITEM* items, int nitems);



#include "Eagle/Image.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Gui/Layout/ClassicMenuLayout.hpp"

std::shared_ptr<ClassicMenuBar> MakeMenuBar(SIMPLE_MENU_ITEM* items , int nitems) {
   std::shared_ptr<ClassicMenuBar> mbar = new ClassicMenuBar(
}
std::shared_ptr<ClassicMenu> MakeMenu(SIMPLE_MENU_ITEM* items, int nitems) {
   
}



#endif // SimpleMenu_HPP
