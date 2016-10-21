

#ifndef TestMenu_HPP
#define TestMenu_HPP



#include "TestRegistry.hpp"


#include "Eagle/Gui/Layout/PinLayout.hpp"




class WedgeLayout : public PinLayout {

protected :
   Pos2d head_pt;
   Pos2d tail_pt1;
   Pos2d tail_pt2;
   
   void ResetPositions() {
      int sz = GetLayoutSize();
      if (!sz) {return;}
      if (sz == 1) {
         SetPinPosition(0 , head_pt.X() , head_pt.Y());
         return;
      }
      
      Pos2d vec1 = head_pt - tail_pt1;
      Pos2d vec2 = tail_pt2 - head_pt;
      
      int half = sz/2 + 1;
      for (int i = 0 ; i < half ; ++i) {
         double pct = i/(double)(half - 1);
         
      }
      for (int i = half  ; i < sz ; ++i) {
            
      }
      
   }
      
   
};




#endif // TestMenu_HPP



