
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Color.hpp
 * @brief The interface for EagleColor objects
 * 
 */

#ifndef EagleColor_HPP
#define EagleColor_HPP



#include <iostream>

extern bool pre_multiply_alpha;

void PreMultiplyAlpha(bool on);///< Set to on to pre multiply alpha when creating an EagleColor object

/**! @class EagleColor
 *   @brief A simple class to create colors
 *
 *   EagleColor has a string constructor. The format is one of the following :
 *   "RGB %i,%i,%i"
 *   OR
 *   "RGBA %i,%i,%i,%i"
 *   OR
 *   "FRGB %f,%f,%f"
 *   OR
 *   "FRGBA %f,%f,%f,%f"
 *
 *   %i represents a textual integer, and %f represents a textual float
 */

class EagleColor {
   
public :
   int r,g,b,a;///< Integral red, green, blue, and alpha
   float fr,fg,fb,fa;///< Floating point red, green, blue, and alpha

   EagleColor();///< Empty constructor, creates black with zero alpha
   EagleColor(std::string colorstr);///< See @ref EagleColor

   ///< EagleColor constructor from r,g,b or r,g,b,a values
   EagleColor(int red , int green , int blue , int alpha = 255);

   ///< EagleColor constructor from floating point r,g,b or r,g,b,a values
   EagleColor(float red , float green , float blue , float alpha = 1.0);

   /// Setters
   
   ///< Set the color and whether or not to apply alpha or pre multiply the color
   void SetColor(int red , int green , int blue , int alpha = 255 , bool mult_alpha = pre_multiply_alpha);
   
   ///< Set the floating point color and whether or not to apply alpha or pre multiply the color
   void SetFloatColor(float red , float green , float blue , float alpha = 1.0 , bool mult_alpha = pre_multiply_alpha);

   void SetAlpha(int alpha);
   void SetFloatAlpha(float alpha);
   
   /// Getters

   int R() const {return r;}
   int G() const {return g;}
   int B() const {return b;}
   int A() const {return a;}
   float fR() const {return fr;}
   float fG() const {return fg;}
   float fB() const {return fb;}
   float fA() const {return fa;}

   friend std::ostream& operator<<(std::ostream& os , const EagleColor& c);///< Ouput an EagleColor to a stream

   inline EagleColor operator*(float alpha) {
      return EagleColor(fr*alpha , fg*alpha , fb*alpha , alpha);
   }
};



#endif // EagleColor_HPP



