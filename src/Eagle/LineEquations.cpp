
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




#include "Eagle/LineEquations.hpp"
#include "Eagle/Exception.hpp"


#include "Eagle/Math.hpp"



void GLE2D::SetABC() {
   Pos2D dz = epos - spos;
   EAGLE_ASSERT(dz.Length() > 0.0f);
   double dy = dz.Y();
   double dx = dz.X();
   a = -dy;
   b = dx;
   c = epos.X()*spos.Y() - epos.Y()*spos.X();
}



GLE2D::GLE2D(Pos2D p , double theta_rad) :
      a(0.0),
      b(0.0),
      c(0.0),
      spos(p),
      epos()
{
   epos = Vector(spos , 1000.0 , theta_rad);
   SetABC();
}



GLE2D::GLE2D(Pos2D p1 , Pos2D p2) :
      a(0.0),
      b(0.0),
      c(0.0),
      spos(p1),
      epos(p2)
{
   SetABC();
}



bool IntersectionPoint(const GLE2D& la , const GLE2D& lb , Pos2D* i) {
   double A = la.A();
   double B = la.B();
   double C = la.C();
   double D = lb.A();
   double E = lb.B();
   double F = lb.C();
   
   double DENOM = A*E-B*D;
   if (DENOM != 0.0) {
      double y = (A*F - C*D)/DENOM;
      double x = (C*E - B*F)/DENOM;
      if (i) {
         i->SetPos(x,y);
      }
      return true;
   }
   return false;
}



double DistanceFromLine(Pos2D p , GLE2D l) {
   double theta = l.SPos().AngleToPoint(l.EPos());
   theta += M_PI/2.0;
   GLE2D line_intersect = GLE2D(p , theta);
   Pos2D i;
   bool intersect = IntersectionPoint(l , line_intersect , &i);
   (void)intersect;
   EAGLE_ASSERT(intersect);
   return (p - i).Length();
}

