
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Vec3.hpp
 * @brief A simple vector class capable of storing xyz, side forward up, and yaw pitch roll triplets
 * 
 *
 */


#ifndef Vec3_HPP
#define Vec3_HPP


#include <cmath>
#include <cassert>

#include "Eagle/StringWork.hpp"

class Vec3 {
public :
   union {
      double x;
      double lateral;
      double yaw;
   };
   union {
      double y;
      double progress;
      double pitch;
   };
   union {
      double z;
      double elevation;
      double roll;
   };
   
   Vec3() : x(0.0) , y(0.0) , z(0.0) {}
   Vec3(double xpos , double ypos , double zpos) : x(xpos) , y(ypos) , z(zpos) {}
   
   void SetXYZ(double xpos , double ypos , double zpos);
   
   Vec3 operator+(const Vec3& v) const {
      return Vec3(x + v.x , y + v.y , z + v.z);
   }
   Vec3 operator-(const Vec3& v) const {
      return *this + -v;
   }
   inline Vec3 operator-() const {
      return Vec3(-x,-y,-z);
   }
   inline Vec3& operator+=(const Vec3& v) {
      *this = *this + v;
      return *this;
   }
   inline Vec3& operator-=(const Vec3& v) {
      *this = *this - v;
      return *this;
   }
   inline Vec3& operator*=(double factor) {
      *this = *this*factor;
      return *this;
   }
   
   inline double Magnitude() const {return sqrt(MagnitudeSquared());}
   inline double MagnitudeSquared() const {return x*x + y*y + z*z;}
   
   void Normalize() {
      double l = Magnitude();
      assert(l != 0.0);
      *this *= 1.0/l;
   }

   friend Vec3 operator*(const Vec3& lhs , double factor);
   friend Vec3 operator*(double factor , const Vec3& lhs);
   friend Vec3 CrossProduct(const Vec3& a , const Vec3& b);
   friend double DotProduct(const Vec3& a , const Vec3& b);
//   std::string ToString();
std::string ToString() {
   return StringPrintF("{%3.4lf,%3.4lf,%3.4lf}" , x , y , z);
}
};

Vec3 Rotate3D(const Vec3& vec , const Vec3& axis , const double theta_radians);

inline Vec3 operator*(const Vec3& lhs , double factor) {
   return Vec3(factor*lhs.x , factor*lhs.y , factor*lhs.z);
}

inline Vec3 operator*(double factor , const Vec3& lhs) {
   return Vec3(factor*lhs.x , factor*lhs.y , factor*lhs.z);
}

inline Vec3 CrossProduct(const Vec3& a , const Vec3& b) {
   return Vec3(a.y*b.z - a.z*b.y , a.z*b.x - a.x*b.z , a.x*b.y - a.y*b.x);
}

inline double DotProduct(const Vec3& a , const Vec3& b) {
   return a.x*b.x + a.y*b.y + a.z*b.z;
}

//inline Vec3 MidPoint(const Vec3& v1 , const Vec3& v2);
inline Vec3 MidPoint(const Vec3& v1 , const Vec3& v2) {
   return Vec3((v1.x + v2.x)/2.0 , (v1.y + v2.y)/2.0 , (v1.z + v2.z)/2.0);
}

extern const Vec3 ORIGIN;

bool ScanVec3(const char* str , Vec3& v);

extern const Vec3 UP;
extern const Vec3 RIGHT;
extern const Vec3 FORWARD;

#endif // Vec3_HPP



