



#include "Turret.hpp"

#include <cmath>


#include "Eagle/GraphicsContext.hpp"


void Turret::Place(int x , int y) {
   xpos = x;
   ypos = y;
}



void Turret::Aim(int x , int y) {
   aimx = x;
   aimy = y;
}



void Turret::Power(int p) {
   if (p < 1) {p = 1;}
   if (p > 50) {p = 100;}
   aimpower = p;
}
   


void Turret::DisplayOn(EagleGraphicsContext* win) {
   /// Draw turret at xpos,ypos
   double angle = atan2(aimy - ypos , aimx - xpos);
   win->DrawLine(xpos , ypos , xpos + 30.0*cos(angle) , ypos + 30.0*sin(angle) , 2.0 , EagleColor(255,255,255));
   /// Draw crosshairs at aimx,aimy
   win->DrawCircle(aimx , aimy , 20 , 1.5 , EagleColor(255,255,255));
   
}
