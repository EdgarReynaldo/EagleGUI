



#include "Terrain.hpp"
#include "RNG.hpp"



Terrain::Terrain() :
      spans()
{
   
}



void Terrain::Clear() {
   spans.clear();
}



void Terrain::Generate(int sw , int sh) {
   Clear();
   spans.resize(sw , VSPAN());
   for (int xpos = 0 ; xpos < sw ; ++xpos) {
      spans[xpos].x = xpos;
      spans[xpos].y = sh;
      spans[xpos].h = (double)(sh/2 - 100);
      spans[xpos].h *= rng.DPercent();
   }
}



void Terrain::DrawOn(EagleGraphicsContext* win) {
   for (unsigned int s = 0 ; s < spans.size() ; ++s) {
      VSPAN v = spans[s];
      win->DrawLine(v.x , v.y , v.x , v.y - v.h , EagleColor(255,255,255));
   }
}



