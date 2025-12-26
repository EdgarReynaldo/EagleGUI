



#include "Artillery.hpp"
#include "Projectile.hpp"
#include "Games.hpp"
#include "ArtilleryGame.hpp"

#include "Eagle/Area.hpp"

Artillery::Artillery() :
      cx(0.0),
      cy(0.0),
      area(0),
      fillcolor(),
      drawcolor()
{
   
}



void Artillery::Setup(double cxpos , double cypos , AreaBase* shape , EagleColor fill , EagleColor draw) {
   cx = cxpos;
   cy = cypos;
   area = shape;
   fillcolor = fill;
   drawcolor = draw;
}


   
void Artillery::Launch(double angle_degrees , double power) {
   Projectile* p = new Projectile(cx , cy , angle_degrees , power);
   agame->AddProjectile(p);
}



void Artillery::DisplayOn(EagleGraphicsContext* win) {
   area->Fill(win , fillcolor);
   area->Draw(win , drawcolor);
}







