



#ifndef Vec2_HPP
#define Vec2_HPP


#include <cmath>
#include <cassert>


class Vec2 {
public :
   union {
      double x;
      double u;
   };
   union {
      double y;
      double v;
   };
   
   Vec2() : x(0.0) , y(0.0) {}
   Vec2(double xpos , double ypos) : x(xpos) , y(ypos) {}
   
   Vec2 operator+(const Vec2& v1) const {
      return Vec2(x + v1.x , y + v1.y);
   }
   Vec2 operator-(const Vec2& v1) const {
      return Vec2(x - v1.x , y - v1.y);
   }
   inline Vec2 operator-() const {
      return Vec2(-x , -y);
   }
   inline Vec2& operator +=(const Vec2& v1) {
      *this = *this + v1;
      return *this;
   }
   inline Vec2& operator-=(const Vec2& v1) {
      *this = *this - v1;
      return *this;
   }
   inline Vec2& operator*=(double factor) {
      *this = *this*factor;
      return *this;
   }
   
   inline double Magnitude() const {return sqrt(MagnitudeSquared());}
   inline double MagnitudeSquared() const {return x*x + y*y;}
   
   void Normalize() {
      double l = Magnitude();
      assert(l != 0.0);
      *this *= 1.0/l;
   }

   inline friend Vec2 operator*(const Vec2& lhs , double factor);
   inline friend Vec2 MidPoint(const Vec2& v1 , const Vec2& v2);
};



inline Vec2 operator*(const Vec2& lhs , double factor) {
   return Vec2(lhs.x*factor , lhs.y*factor);
}



inline Vec2 MidPoint(const Vec2& v1 , const Vec2& v2) {
   return Vec2((v1.x + v2.x)/2.0 , (v1.y + v2.y)/2.0);
}



#endif // Vec2_HPP



