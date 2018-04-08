



#ifndef LineEquations_HPP
#define LineEquations_HPP


#include "Eagle/Position.hpp"



class GLE2D {
   /// Ax + By + C = 0
   double a;
   double b;
   double c;
   
   Pos2D spos;
   Pos2D epos;
   
   void SetABC();

public :
   GLE2D(Pos2D p , double theta_rad);
   GLE2D(Pos2D p1 , Pos2D p2);
   
   
   double A() const {return a;}
   double B() const {return b;}
   double C() const {return c;}
   Pos2D SPos() const {return spos;}
   Pos2D EPos() const {return epos;}
};

bool IntersectionPoint(const GLE2D& la , const GLE2D& lb , Pos2D* i);

double DistanceFromLine(Pos2D p , GLE2D l);

#endif // LineEquations_HPP

