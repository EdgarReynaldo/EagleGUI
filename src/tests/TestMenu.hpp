

#ifndef TestMenu_HPP
#define TestMenu_HPP



#include "TestRegistry.hpp"


#include "Eagle/Gui/Layout/PinLayout.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"


#include "TestRegistry.hpp"


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
   EagleGraphicsContext* window;
   WidgetHandler gui;
   WedgeLayout wedge_layout;
   RelativeLayout relative_layout;
   
   vector<BasicButton*> buttons;
   
   BasicButton* quit_button;
   
   EagleFont* font;
   
   bool quit;
   const char* selected_user_branch;
   
   
public :
   TestMenu(EagleGraphicsContext* win) :
         window(win),
         gui(),
         wedge_layout(),
         relative_layout(),
         buttons(),
         quit_button(0),
         font(0),
         quit(false),
         selected_user_branch(0)
   {
      EAGLE_ASSERT(win);
      EAGLE_ASSERT(win->Valid());

      int w = win->Width();
      int h = win->Height();
      
      font = win->LoadFont("Verdana.ttf" , -20);
      EAGLE_ASSERT(font->Valid());
      
      gui.SetupBufferDimensions(w,h);
      gui.SetRootLayout(&relative_layout);
      
      relative_layout.AddWidget(quit_button , LayoutRectangle(0.1 , 0.4 , 0.4 , 0.2));
      relative_layout.AddWidget(wedge_layout , LayoutRectangle(0.0 , 0.0 , 1.0 , 1.0));
      
      vector<const Test*> tests = test_registry.GetRegisteredTests();
      
      wedge_layout.SetAlignment(HALIGN_CENTER , VALIGN_CENTER);
      wedge_layout.SetAnchors(Pos2d(3*w/4 , h/2) , Pos2d(w/4 , h/4) , Pos2d(w/4 , 3*h/4));
      wedge_layout.Resize(tests.size());
      buttons.resize(tests.size());
      for (int i = 0 ; i < (int)tests.size() ; ++i) {
         TextButton* btn = CreateWidget<TextButton>("TextButton" , "");
         EAGLE_ASSERT(btn);
         btn->SetText
      }
      
   }
   
   void Run();
   void Run() {
      
   }
   
   bool Quit() {return quit;}
   
}


    

#endif // TestMenu_HPP



