

#ifndef TestMenu_HPP
#define TestMenu_HPP



#include "TestRegistry.hpp"

#include "Eagle/Gui/Layout/RelativeLayout.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"
#include "Eagle/Gui/Button/TextButton.hpp"
#include "Eagle/Animations.hpp"
#include "Eagle/GraphicsContext.hpp"

#include "TestRegistry.hpp"
#include "AnimatedWedgeLayout.hpp"


#include <vector>
using std::vector;


class TestMenu {
   
protected :
   EagleGraphicsContext* win;
   WidgetHandler gui;
   AnimatedWedgeLayout wedge_layout;
   
   RelativeLayout relative_layout;
   
   vector<TextButton*> buttons;
   
   TextButton* quit_button;
   
   BasicButton* basic_button;
   
   EagleFont* font;
   
   bool quit;
   bool selected;
   string selected_user_branch;
   
   string status_message;
   
   
public :
   TestMenu(EagleGraphicsContext* window);
   
   
   void Run();
   
   bool Quit() {return quit;}
   
   string SelectedBranch() {return selected_user_branch;}
   
   void SetStatusMessage(std::string status);
   
   void PrintQuitButton() {EagleLog() << *quit_button << std::endl;}
   void PrintGui() {EagleLog() << gui << std::endl;}
   
};


    

#endif // TestMenu_HPP



