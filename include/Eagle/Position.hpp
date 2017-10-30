


#ifndef Position2T_HPP
#define Position2T_HPP

#include <iostream>
#include <cmath>

#include "Eagle/Color.hpp"


class EagleGraphicsContext;
///#include "Eagle/GraphicsContext.hpp"



template <class TYPE>
class Pos2T {
   
   TYPE tx;
   TYPE ty;
   
public :
   Pos2T() : tx(0) , ty(0) {}
   Pos2T(const TYPE& x , const TYPE& y) : tx(x) , ty(y) {}
   
   void SetPos(TYPE xpos , TYPE ypos)    {tx = xpos;ty=ypos;}
   Pos2T& MoveBy(TYPE dx , TYPE dy)      {SetPos(tx + dx , ty + dy);return *this;}
   Pos2T& MoveBy(const Pos2T& p)         {return MoveBy(p.X() , p.Y());}

   bool operator==(const Pos2T& p) const {return ((p.tx == tx) && (p.ty == ty));}
   
   Pos2T operator-(const Pos2T& p) const {
      return Pos2T(tx - p.X() , ty - p.Y());
      return *this;
   }

   Pos2T operator+(const Pos2T& p) const {
      return Pos2T(tx + p.X() , ty + p.Y());
      return *this;
   }
   
   Pos2T& operator-() const {
      *this = Pos2T(-tx , -ty);
      return *this;
   }
   
   Pos2T& operator-=(const Pos2T& add) {MoveBy(-add);}
   Pos2T& operator+=(const Pos2T& add) {MoveBy(add);}
   
   Pos2T& operator*(double scale) {*this = Pos2T(scale*tx , scale*ty);return *this;}
   
///   void Draw(EagleGraphicsContext* win , EagleColor color) const;

   void DrawLineTo(EagleGraphicsContext* win , const Pos2T& p , EagleColor color) const;
   void DrawLineTo(EagleGraphicsContext* win , const Pos2T& p , double thickness , EagleColor color) const ;

/*

   double DistanceToPoint(const Pos2T& p) const;
   double DistanceToPoint(TYPE xpos , TYPE ypos) const;
*/
   double AngleToPoint(const Pos2T& p) const {return atan2(p.Y() - ty , p.X() - tx);}
   double AngleToPoint(TYPE x , TYPE y) const {return atan2(y - ty , x - tx);}

   TYPE X() const {return tx;}
   TYPE Y() const {return ty;}
   
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

Pos2D Vector(const Pos2D& p , double r , double theta);

#endif // Position2T_HPP

