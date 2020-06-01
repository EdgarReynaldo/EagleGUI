



#include "eagle/Transitions.hpp"



void Interpolation::Recalc(double pct) {
   if (pct < 0.0) {
      pct = 0.0;
   }
   if (pct > 1.0) {
      pct = 1.0;
   }
   Recalculate(pct);
}



void Translate2::Recalculate(double pct) {
   cpt = Interpolate(spt , dpt , pct);
}



Translate2::Translate2(Pos2D src , Pos2D dest) :
      spt(src),
      dpt(dest),
      cpt(src)
{}



Pos2D Translate2::GetPosition() {return cpt;}



void Scale2::Recalculate(double pct) {
   cscale = Interpolate(sscale , dscale , pct);
}



Scale2::Scale2(Pos2D src_scale , Pos2D dest_scale) :
      sscale(src_scale),
      dscale(dest_scale),
      cscale(src_scale)
{}



Pos2D Scale2::GetScale() {return cscale;}



void Rotate2::Recalculate(double pct) {
   ctheta = Interpolate(stheta , dtheta , pct);
}



Rotate2::Rotate2(double start_theta , double dest_theta) :
      stheta(start_theta),
      dtheta(dest_theta),
      ctheta(start_theta)
{}



double Rotate2::GetRotation() {return ctheta;}



void Transition::Recalculate(double pct) {
   move.Recalc(pct);
   size.Recalc(pct);
}



Transition::Transition(Pos2D startpos , Pos2D endpos , Pos2D startsize , Pos2D endsize) :
      move(startpos , endpos),
      size(startsize , endsize)
{}


