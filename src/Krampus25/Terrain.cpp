



#include "Terrain.hpp"
#include "RNG.hpp"
#include "Eagle/Math.hpp"
#include "Eagle/GraphicsContext.hpp"

Terrain::Terrain() :
      spans()
{
   
}



void Terrain::Clear() {
   spans.clear();
}



void Terrain::Generate(int sw , int sh) {
   Clear();
   spans.resize(sw , VSPAN());/// One VSPAN for every x in sw
   double htleft = 0.0*sh*rng.DPercent();/// From 0.0 to 1.0, representing height on left side, limited to on screen;
   double htright = sh*rng.DPercent();/// From 0.0 to 1.0, representing height on right side, limited to on screen;
   double ht = htleft;
   double slope = 5.0;
   double sign = 1.0;
   double accel = -2.0;
   double asign = 1.0;
   for (int x = 0 ; x < sw ; ++x) {
      spans[x].x = x;
      spans[x].y = sh - (int)ht;
      spans[x].h = ht;
      ht += sin(slope*M_PI/180.0);
      if (ht < 0) {
         ht *= -1;
      }
//      ht = ((1.0 - x/(double)sw)*ht + htright*(double)x/(double)sw);
      slope += sign + asign*accel;
      accel += asign;
      if (rng.DPercent() < 0.05) {
         sign *= -1.0;
      }
      if (rng.DPercent() < 0.2) {
         asign *= -1.0;
      }
   }
}
/**
void Terrain::Generate2(int sw , int sh) {
   Clear();
   double baseh = 100.0;
   double slope = 5.0;
   double sign = 1.0;
   double accel = -1.0;
   double asign = 0.2;
   spans.resize(sw , VSPAN());
   for (int xpos = 0 ; xpos < sw ; ++xpos) {
      spans[xpos].x = xpos;
      spans[xpos].y = sh;
      spans[xpos].h = baseh;
      baseh += sin(slope*M_PI/180.0);
      slope += sign + asign*accel;
      accel += asign;
      if (rng.DPercent() < 0.05) {
         sign *= -1.0;
      }
      if (rng.DPercent() < 0.2) {
         asign *= -1.0;
      }
   }
}
*/


void Terrain::DisplayOn(EagleGraphicsContext* win) {
   for (unsigned int s = 0 ; s < spans.size() ; ++s) {
      VSPAN v = spans[s];
      win->DrawLine(v.x , v.y , v.x , v.y + v.h , EagleColor(255,255,255));
   }
}



