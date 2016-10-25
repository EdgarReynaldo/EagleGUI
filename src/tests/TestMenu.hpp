

#ifndef TestMenu_HPP
#define TestMenu_HPP



#include "TestRegistry.hpp"


#include "Eagle/Gui/Layout/PinLayout.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"



class WedgeLayout : public PinLayout {

protected :
   Pos2d head_pt;
   Pos2d tail_pt1;
   Pos2d tail_pt2;
   
public :
   
   WedgeLayout() : PinLayout("Specialized Wedge Layout") {}
   
   void SetAnchors(Pos2d head , Pos2d tail1 , Pos2d tail2);
   void ResetPositions();

      
   
};


#include <vector>
using std::vector;


class TestMenu {
   
protected :
   WidgetHandler gui;
   WedgeLayout wedge_layout;
   
   vector<BasicButton*> buttons;
   
   BasicButton* quit_button;
   
   bool quit;
   const char* selected_user_branch;
   
   
public :
   TestMenu() :
         gui(),
         wedge_layout(),
         buttons(),
         quit_button(0),
         quit(false),
         selected_user_branch(0)
   {
      
   }
   
   bool Quit() {return quit;}
   
}


    

#endif // TestMenu_HPP



