



#include "Artillery.hpp"
#include "Projectile.hpp"
#include "Games.hpp"
#include "ArtilleryGame.hpp"

#include "Eagle/Area.hpp"

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


   
void Artillery::Launch(double angle_degrees , double pps , double power) {
   Projectile* p = new Projectile(cx , cy , angle_degrees , pps , power);
   agame->AddProjectile(p);
}



