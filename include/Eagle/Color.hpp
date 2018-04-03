
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleColor_HPP
#define EagleColor_HPP


#include <iostream>



extern bool pre_multiply_alpha;
void PreMultiplyAlpha(bool on);

class EagleColor {
   
public :
   int r,g,b,a;
   float fr,fg,fb,fa;

   EagleColor();
   EagleColor(int red , int green , int blue , int alpha = 255);
   EagleColor(float red , float green , float blue , float alpha = 1.0);

   // Setters
   void SetColor(int red , int green , int blue , int alpha = 255 , bool mult_alpha = pre_multiply_alpha);
   void SetFloatColor(float red , float green , float blue , float alpha = 1.0 , bool mult_alpha = pre_multiply_alpha);

//**
   // Getters
   int R() {return r;}
   int G() {return g;}
   int B() {return b;}
   int A() {return a;}
   float fR() {return fr;}
   float fG() {return fg;}
   float fB() {return fb;}
   float fA() {return fa;}
//*/
   friend std::ostream& operator<<(std::ostream& os , const EagleColor& c);

};

#endif // EagleColor_HPP

