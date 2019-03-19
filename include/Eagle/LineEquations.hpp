
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

