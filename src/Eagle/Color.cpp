
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Color.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"

#include <cstring>

bool pre_multiply_alpha = true;

void PreMultiplyAlpha(bool on) {
   pre_multiply_alpha = on;
}



EagleColor::EagleColor() :
      r(0),
      g(0),
      b(0),
      a(0),
      fr(0.0f),
      fg(0.0f),
      fb(0.0f),
      fa(0.0)
{}



EagleColor::EagleColor(std::string colorstr) :
      r(255),
      g(0),
      b(255),
      a(255),
      fr(1.0f),
      fg(0.0f),
      fb(1.0f),
      fa(1.0f)
{
   const char* str = SkipWhiteSpace(colorstr.c_str());
   if (strncmp(str , "RGBA " , 5) == 0) {
      int r1,g1,b1,a1;
      if (4 == sscanf(str , "RGBA %i,%i,%i,%i" , &r1 , &g1 , &b1 , &a1)) {
         SetColor(r1,g1,b1,a1);
      }
   }
   else if (strncmp(str , "FRGBA " , 6) == 0) {
      float fr1,fg1,fb1,fa1;
      if (4 == sscanf(str , "FRGBA %f,%f,%f,%f" , &fr1 , &fg1 , &fb1 , &fa1)) {
         SetFloatColor(fr1,fg1,fb1,fa1);
      }
   }
   else {
      throw EagleException(StringPrintF("EagleColor(std::string) : Failed to parse EagleColor '%s'\n" , colorstr.c_str()));
   }
}



EagleColor::EagleColor(int red , int green , int blue , int alpha) :
      r(0),
      g(0),
      b(0),
      a(0),
      fr(0.0f),
      fg(0.0f),
      fb(0.0f),
      fa(0.0)
{
   SetColor(red,green,blue,alpha);
}



EagleColor::EagleColor(float red , float green , float blue , float alpha) :
      r(0),
      g(0),
      b(0),
      a(0),
      fr(0.0f),
      fg(0.0f),
      fb(0.0f),
      fa(0.0)
{
   SetFloatColor(red,green,blue,alpha);
}



void EagleColor::SetColor(int red , int green , int blue , int alpha , bool mult_alpha) {
   if (red < 0) {red = 0;}
   if (green < 0) {green = 0;}
   if (blue < 0) {blue = 0;}
   if (alpha < 0) {alpha = 0;}
   
   if (!mult_alpha) {
      r = red;
      g = green;
      b = blue;
      a = alpha;
      fr = (float)r/255.0f;
      fg = (float)g/255.0f;
      fb = (float)b/255.0f;
      fa = (float)a/255.0f;
   }
   else {// multiply alpha
      r = (red*alpha)/255;
      g = (green*alpha)/255;
      b = (blue*alpha)/255;
      a = alpha;
      fr = (float)r/255.0f;
      fg = (float)g/255.0f;
      fb = (float)b/255.0f;
      fa = (float)a/255.0f;
   }
   
}



void EagleColor::SetFloatColor(float red , float green , float blue , float alpha , bool mult_alpha) {
   if (!mult_alpha) {
      fr = red;
      fg = green;
      fb = blue;
      fa = alpha;
      r = (int)(255.0*fr);
      g = (int)(255.0*fg);
      b = (int)(255.0*fb);
      a = (int)(255.0*fa);
   }
   else {// multiply alpha
      fr = red*alpha;
      fg = green*alpha;
      fb = blue*alpha;
      fa = alpha;
      r = (int)(255.0*fr);
      g = (int)(255.0*fg);
      b = (int)(255.0*fb);
      a = (int)(255.0*fa);
   }
}



void EagleColor::SetAlpha(int alpha) {
   SetColor(r,g,b,alpha,false);
}



void EagleColor::SetFloatAlpha(float alpha) {
   SetFloatColor(fr,fg,fb,alpha , false);
}



std::ostream& operator<<(std::ostream& os , const EagleColor& c) {
   os << "RGBA = " << c.r << "," << c.g << "," << c.b << "," << c.a;
   return os;
}



