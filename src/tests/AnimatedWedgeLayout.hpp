



#ifndef AnimatedWedgeLayout_HPP
#define AnimatedWedgeLayout_HPP

#include "Eagle/Position.hpp"
#include "Eagle/AnimationBase.hpp"
#include "Eagle/Gui/Layout/PinLayout.hpp"



class AnimatedWedgeLayout : public PinLayout , public AnimationBase {

protected :
   Pos2I head_pt;
   Pos2I tail_pt1;
   Pos2I tail_pt2;
   
   bool animation_running;
   bool direction_forward;
   double move_percent;
   
   
   /// From AnimationBase
   virtual void OnSetAnimationPercent();

   virtual void OnLoopComplete();
   virtual void OnComplete();

   /// From Layout
   virtual void ReserveSlots(int nslots);

   


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
   void SetAnchors(Pos2I head , Pos2I tail1 , Pos2I tail2);
   void ResetPositions();

   /// PinLayout, Layout overloads
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
      
   /// WidgetBase overloads
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int Update(double tsec);
   
};



#endif // AnimatedWedgeLayout_HPP






