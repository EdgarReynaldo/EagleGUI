




#include "TestMenu.hpp"




void SetAnchors(Pos2d head , Pos2d tail1 , Pos2d tail2) {
   head_pt = head;
   tail_pt1 = tail1;
   tail_pt2 = tail2;
   ResetPositions();
}



void ResetPositions() {
   int sz = GetLayoutSize();
   if (!sz) {return;}
   if (sz == 1) {
      SetPinPosition(0 , head_pt.X() , head_pt.Y());
      return;
   }
   
   /// Widgets are laid out in a wedge from tail 1 to head to tail 2, spread out evenly
   Pos2d vec1 = head_pt - tail_pt1;
   Pos2d vec2 = tail_pt2 - head_pt;
   
   int half = sz/2 + 1;
   for (int i = 0 ; i < half ; ++i) {
      double pct = i/(double)(half - 1);
      Pos2d v = tail_pt1 + vec1*pct;
      SetPinPosition(i , v.X() , v.Y());
   }
   for (int i = half  ; i < sz ; ++i) {
      double pct = (i - half + 1)/(double)(sz - half);
      Pos2d v = head_pt + vec2*pct;
      SetPinPosition(i , v.X() , v.Y());
   }
   RepositionAllChildren();
}


