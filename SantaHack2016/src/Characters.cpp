


#include "Characters.hpp"







void Character::SetDirection(DIRECTION d) {
   int ydir = 0;
   if ((int)d < 3) {ydir = -1;}
   else if ((int)d > 5) {ydir = 1;}
   int xdir = 0;
   if ((int)d%3 == 0) {xdir = -1;}
   else if ((int)d%3 == 2) {xdir = 1;}
   TryToMove(xdir , ydir , eagle_system->GetSystemTimer()->SPT());
}



void Character::TryToMove(int xdir , int ydir , double dt) {
   if (xdir < 0) {xdir = -1;}
   if (xdir > 0) {xdir = 1;}
   if (ydir < 0) {ydir = -1;}
   if (ydir > 0) {ydir = 1;}
   
   nx = cx + (float)xdir*max_vx*dt;
   ny = cy + (float)ydir*max_vy*dt;
}



void Character::Draw160(EagleGraphicsContext* win , int xpos , int ypos) {
   win->Draw(image_160 , xpos , ypos);
}



void Character::Draw64(EagleGraphicsContext* win , int xpos , int ypos) {
   win->Draw(image_64 , xpos , ypos);
}



void Character::Draw(EagleGraphicsContext* win) {
   win->Draw(image_64 , cx - 32.0 , cy - 32.0);
}



float Character::CX() {return cx;}



float Character::CY() {return cy;}



float Character::NX() {return nx;}



float Character::NY() {return ny;}





/// -------------------------      Human      -----------------------------------------





void Human::Draw32(EagleGraphicsContext* win , int xpos , int ypos) {
   win->Draw(image_32 , xpos , ypos);
}



void Human::Draw32BW(EagleGraphicsContext* win , int xpos , int ypos) {
   win->Draw(image_32bw , xpos , ypos);
}
















