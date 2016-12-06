

#ifndef TestMenu_HPP
#define TestMenu_HPP



#include "TestRegistry.hpp"

#include "Eagle/Gui/Layout/RelativeLayout.hpp"
#include "Eagle/Gui/Layout/PinLayout.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"
#include "Eagle/Gui/Button/TextButton.hpp"
#include "Eagle/Animations.hpp"
#include "Eagle/GraphicsContext.hpp"

#include "TestRegistry.hpp"


class AnimatedWedgeLayout : public PinLayout , public AnimationBase {

protected :
   Pos2d head_pt;
   Pos2d tail_pt1;
   Pos2d tail_pt2;
   
   bool animation_running;
   bool direction_forward;
   double move_percent;
   
   
   virtual void OnSetAnimationPercent();

   virtual void OnLoopComplete();
   virtual void OnComplete();



public :
   
///   WedgeLayout();
   AnimatedWedgeLayout() : 
         PinLayout("Animated Wedge Layout"),
         AnimationBase(),
         head_pt(),
         tail_pt1(),
         tail_pt2(),
         animation_running(false),
         direction_forward(false),
         move_percent(0.0)
   {
      
   }
   
   /// AnimatedWedgeLayout
   void SetAnchors(Pos2d head , Pos2d tail1 , Pos2d tail2);
   void ResetPositions();

   /// PinLayout, Layout overloads
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
      
   /// WidgetBase overloads
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int Update(double tsec);
   
};


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
   
   EagleFont* font;
   
   bool quit;
   bool selected;
   string selected_user_branch;
   
   
public :
   TestMenu(EagleGraphicsContext* window);
   
   
   void Run();
   
   bool Quit() {return quit;}
   
   string SelectedBranch() {return selected_user_branch;}
   
   
   void PrintQuitButton() {EagleLog() << *quit_button << std::endl;}
   void PrintGui() {EagleLog() << gui << std::endl;}
   
};


    

#endif // TestMenu_HPP



