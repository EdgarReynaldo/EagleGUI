


#ifndef Position_HPP
#define Position_HPP

#include <iostream>
#include <cmath>


#include "Eagle/Color.hpp"


class EagleGraphicsContext;


/// -------------------------------------     Pos2d class     ------------------------------------------



class Pos2d {
protected :
   int x;
   int y;
   
public :
   Pos2d() : x(0) , y(0) {}
   Pos2d(int xpos , int ypos) : x(xpos) , y(ypos) {}
   
   inline bool operator==(const Pos2d& p) const ;
   inline Pos2d& operator-=(const Pos2d& add);
   inline Pos2d& operator+=(const Pos2d& add);
   
   inline void SetPos(int xpos , int ypos);
   inline Pos2d& MoveBy(int dx , int dy);

   void Draw(EagleGraphicsContext* win , EagleColor color) const;
   void DrawLineTo(EagleGraphicsContext* win , const Pos2d& p , EagleColor color) const;
   void DrawLineTo(EagleGraphicsContext* win , const Pos2d& p , double thickness , EagleColor color) const ;

   inline double AngleToPoint(const Pos2d& p) const;
   inline double AngleToPoint(int x , int y) const;

   inline double DistanceToPoint(const Pos2d& p) const;
   inline double DistanceToPoint(int xpos , int ypos) const;

   int X() const {return x;}
   int Y() const {return y;}
   
   friend std::ostream& operator<<(std::ostream& os , const Pos2d& p);
};



inline bool Pos2d::operator==(const Pos2d& p) const {
   return ((x == p.x) && (y == p.y));
}

inline Pos2d& Pos2d::operator-=(const Pos2d& add) {
   this->MoveBy(-add.X() , -add.Y());
   return *this;
}

inline Pos2d& Pos2d::operator+=(const Pos2d& add) {
   this->MoveBy(add.X() , add.Y());
   return *this;
}

inline void Pos2d::SetPos(int xpos , int ypos) {
   x = xpos;
   y = ypos;
}

inline Pos2d& Pos2d::MoveBy(int dx , int dy) {
   SetPos(x + dx , y + dy);
   return *this;
}

inline double Pos2d::AngleToPoint(const Pos2d& p) const {
   if (*this == p) {return 0.0;}// same point, no angle
   return atan2(p.y - y , p.x - x);
}

inline double Pos2d::AngleToPoint(int xpos , int ypos) const {
   return atan2(ypos - y , xpos - x);
}

inline double Pos2d::DistanceToPoint(const Pos2d& p) const {
   int dx = p.x - x;
   int dy = p.y - y;
   return sqrt((double)(dx*dx + dy*dy));
}

inline double Pos2d::DistanceToPoint(int xpos , int ypos) const {
   int dx = xpos - x;
   int dy = ypos - y;
   return sqrt((double)(dx*dx + dy*dy));
}

inline double AngleToP2(const Pos2d& p1 , const Pos2d& p2) {return p1.AngleToPoint(p2);}
   
inline Pos2d operator-(const Pos2d& lhs , const Pos2d rhs) {
   return Pos2d(lhs.X() - rhs.X() , lhs.Y() - rhs.Y());
}

inline Pos2d operator*(const Pos2d& lhs , const double& factor) {
   double x = (double)lhs.X()*factor;
   double y = (double)lhs.Y()*factor;
   return Pos2d((int)x , (int)y);
}

inline Pos2d operator*(const double& factor , const Pos2d& rhs) {
   double x = factor*(double)rhs.X();
   double y = factor*(double)rhs.Y();
   return Pos2d((int)x , (int)y);
}

inline Pos2d operator+(const Pos2d& lhs , const Pos2d& rhs) {
   return Pos2d(lhs.X() + rhs.X() , lhs.Y() + rhs.Y());
}


#endif // Position_HPP
