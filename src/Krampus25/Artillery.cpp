



#include "Artillery.hpp"



Artillery::Artillery() :
      cx(0.0),
      cy(0.0),
      area(0),
      color()
{
   
}



void Artillery::Setup(double cxpos , double cypos , AreaBase* shape , EagleColor c) {
   cx = cxpos;
   cy = cypos;
   area = shape;
   color = c;
}


   
bool Artillery::Launch(double angle_degrees , double pps , double power) {
   Projectile* p = new Projectile(double x , double y , double angle_degrees , double velocity , double power);
   artillery_game->AddProjectile(p);
}



