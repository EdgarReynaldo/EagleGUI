



#include "Terrain.hpp"
#include "Games.hpp"
#include "RNG.hpp"
#include "Eagle/Math.hpp"
#include "Eagle/GraphicsContext.hpp"

Terrain::Terrain() :
      spans()
{
   
}



int Terrain::HeightAtX(int xpos) {
   if (xpos < 0) {
      return scrh;
   }
   if (xpos >= (int)spans.size()) {
      return scrh;
   }
   if (!spans.size()) {
      return scrh;
   }
   return spans[xpos].h;
}



void Terrain::Clear() {
   spans.clear();
}



void Terrain::Generate(int sw , int sh) {
   Clear();
   spans.resize(sw , VSPAN());/// One VSPAN for every x in scrw
   double htleft = 100 + (sh/4)*rng.DPercent();/// From 0.0 to 1.0, representing height on left side, limited to on screen;
   //double htright = scrh*rng.DPercent();/// From 0.0 to 1.0, representing height on right side, limited to on screen;
   double ht = htleft;
   double slope = rng.Rand0toNM1(45);
   double sign = 3.0;
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
//      ht = ((1.0 - x/(double)scrw)*ht + htright*(double)x/(double)scrw);
      slope += sign + asign*accel;
      accel += asign;
      if (rng.DPercent() < 0.0125) {
//         sign *= -1.0;
         asign *= -1.0;
      }
//      if (rng.DPercent() < 0.04) {
//         asign *= -1.0;
//      }
   }
}
/**
void Terrain::Generate2(int scrw , int scrh) {
   Clear();
   double baseh = 100.0;
   double slope = 5.0;
   double sign = 1.0;
   double accel = -1.0;
   double asign = 0.2;
   spans.resize(scrw , VSPAN());
   for (int xpos = 0 ; xpos < scrw ; ++xpos) {
      spans[xpos].x = xpos;
      spans[xpos].y = scrh;
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



