



#ifndef AnimatedWedgeLayout_HPP
#define AnimatedWedgeLayout_HPP

#include "Eagle/Position.hpp"
#include "Eagle/AnimationBase.hpp"
#include "Eagle/Gui/Layout/PinLayout.hpp"



class AnimatedWedgeLayout : public PinLayout , public AnimationBase {

protected :
   Pos2d head_pt;
   Pos2d tail_pt1;
   Pos2d tail_pt2;
   
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
   void SetAnchors(Pos2d head , Pos2d tail1 , Pos2d tail2);
   void ResetPositions();

   /// PinLayout, Layout overloads
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
      
   /// WidgetBase overloads
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int Update(double tsec);
   
};



#endif // AnimatedWedgeLayout_HPP






