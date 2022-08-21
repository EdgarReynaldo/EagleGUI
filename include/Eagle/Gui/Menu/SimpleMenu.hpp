



#ifndef SimpleMenu_HPP
#define SimpleMenu_HPP


#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/InputHandler.hpp"

class EagleImage;
class EagleFont;

#include <string>


class SIMPLE_MENU_ITEM {
public :
   BUTTON_ACTION_TYPE   batype;
   bool              down;
   EagleImage*       image_up;
   EagleImage*       image_dn;
   EagleFont*        text_font;
   std::string       description;
   InputGroup        hotkey;
   std::string       key_text;
};


class SIMPLE_MENU_BAR_ITEM {
public :
   EagleFont* text_font;
   std::string guitext;
   InputGroup hotkey;
};







#endif // SimpleMenu_HPP
