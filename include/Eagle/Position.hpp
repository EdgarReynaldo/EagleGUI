
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
 * @file Position.hpp
 * @brief Simple template class for working with 2D objects of various types of precision
 * 
 *
 * Pos2T is a template class for holding an x and y position in any given precision.
 *
 * Pos2I is @ref Pos2T<int>
 * Pos2F is @ref Pos2T<float>
 * Pos2D is @ref Pos2T<double>
 *
 * You can add and subtract and apply scalar multiplication. You can get angles between points, draw lines and (TODO : Arrows) and more.
 */

#ifndef Position2T_HPP
#define Position2T_HPP



#include <iostream>
#include <cmath>



#include "Eagle/Color.hpp"


class EagleGraphicsContext;


/**! @class Pos2T <TYPE>
 *   @brief A template class for holding 2D points
 */

template <class TYPE>
class Pos2T {
   
public :
   TYPE tx;
   TYPE ty;
   
   Pos2T() : tx(0) , ty(0) {}
   Pos2T(const TYPE& x , const TYPE& y) : tx(x) , ty(y) {}
   
   void SetPos(TYPE xpos , TYPE ypos)    {tx = xpos;ty=ypos;}

   Pos2T& MoveBy(TYPE dx , TYPE dy)      {SetPos(tx + dx , ty + dy);return *this;}
   Pos2T& MoveBy(const Pos2T& p)         {return MoveBy(p.X() , p.Y());}
   
   Pos2T MovedBy(TYPE dx , TYPE dy) const {Pos2T p = *this;return p.MoveBy(dx,dy);}
   Pos2T MovedBy(const Pos2T& p)    const {Pos2T p2 = *this;return p2.MoveBy(p);}

   bool operator==(const Pos2T& p) const {return ((p.tx == tx) && (p.ty == ty));}
   
   Pos2T operator-(const Pos2T& p) const {
      return Pos2T(tx - p.X() , ty - p.Y());
   }

   Pos2T operator+(const Pos2T& p) const {
      return Pos2T(tx + p.X() , ty + p.Y());
   }
   
   Pos2T operator-() const {
      return Pos2T(-tx , -ty);
   }
   
   Pos2T& operator-=(const Pos2T& add) {return MoveBy(-add);}
   Pos2T& operator+=(const Pos2T& add) {return MoveBy(add);}
   
   Pos2T operator*(double scale) {return Pos2T(scale*tx , scale*ty);}
   Pos2T& operator*=(double scale) {*this = *this * scale;return *this;}
   
   void DrawLineTo(EagleGraphicsContext* win , const Pos2T& p , EagleColor color) const;
   void DrawLineTo(EagleGraphicsContext* win , const Pos2T& p , double thickness , EagleColor color) const ;

   double DistanceToPoint(const Pos2T& p) const {return sqrt((p.X() - tx)*(p.X() - tx) + (p.Y() - ty)*(p.Y() - ty));}
   double DistanceToPoint(TYPE xpos , TYPE ypos) const {return sqrt((xpos - tx)*(xpos - tx) + (ypos - ty)*(ypos - ty));}

   double AngleToPoint(const Pos2T& p) const {return atan2(p.Y() - ty , p.X() - tx);}
   double AngleToPoint(TYPE x , TYPE y) const {return atan2(y - ty , x - tx);}

   double LengthSquared() const {return tx*tx + ty*ty;}
   double Length() const {return sqrt(LengthSquared());}
   
   TYPE X() const {return tx;}
   TYPE Y() const {return ty;}
   
   void Normalize() {*this *= 1.0/Length();}
   
///   friend std::ostream& operator<<(std::ostream& os , const Pos2T<TYPE>& p);
   
};

template <class TYPE>
std::ostream& operator<<(std::ostream& os , const Pos2T<TYPE>& p) {
   os << p.X() << " , " << p.Y();
   return os;
}



template <class TYPE>
Pos2T<TYPE> operator*(double scale , const Pos2T<TYPE>& p) {return Pos2T<TYPE>(scale*p.X() , scale*p.Y());}



typedef Pos2T<int> Pos2I;
typedef Pos2T<float> Pos2F;
typedef Pos2T<double> Pos2D;

Pos2F VectorF(const Pos2F& p , float r , float theta);
Pos2D VectorD(const Pos2D& p , double r , double theta);




#endif // Position2T_HPP




