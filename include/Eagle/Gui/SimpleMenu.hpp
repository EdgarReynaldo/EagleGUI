



#ifndef SimpleMenu_HPP
#define SimpleMenu_HPP


#include "Eagle/Gui/BasicButton.hpp"
#include "Eagle/InputHandler.hpp"

class EagleImage;
class EagleFont;

#include <string>



class SIMPLE_MENU_ITEM {
public :
   BTN_ACTION_TYPE   batype;
   bool              down;
   EagleImage*       image_up;
   EagleImage*       image_dn;
   EagleFont*        text_font;
   std::string       description;
   InputGroup        action_key;
   std::string       key_text;
   SIMPLE_MENU_ITEM* sub_menu;
};





#endif // SimpleMenu_HPP
