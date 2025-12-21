



#include "Projectile.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/WidgetBase.hpp"


Projectile::Projectile(double x , double y , double angle_degrees , double velocity , double power) :
      startxpos(x),
      startypos(y),
      launch_angle_degrees(angle_degrees),
      wind_angle_degrees(0.0),
      vel(velocity),
      velwind(0.0),
      expower(power),
      xpos(x),
      ypos(y),
      velx(0.0),
      vely(0.0),
      windvelx(0.0),
      windvely(0.0),
      gravity(9.8),
      explode(false),
      exp_timer(2.0),
      explosion(),
      frames(),
      map(0)
{
   velx = vel*cos(angle_degrees);
   vely = vel*sin(angle_degrees);
}



Projectile::~Projectile() {
   (void)0;
}



void Projectile::DisplayOn(EagleGraphicsContext* win) {
   if (!explode) {
      win->DrawLine(xpos -1, ypos , xpos + 1 , ypos , 1.0 , EagleColor(255,255,255));
      win->DrawLine(xpos , ypos - 1 , xpos , ypos + 1 , 1.0 , EagleColor(255,255,255));
   }
   else if (exp_timer >= 0.0) {
      int fnum = explosion.GetFrameNum();
      win->Draw(frames[fnum] , xpos , ypos , HALIGN_CENTER , VALIGN_CENTER);
   }
}



int Projectile::Update(double dt) {
   if (dt > 0.0 && !explode) {
      xpos += (velx = cos(launch_angle_degrees)*vel);
      ypos += (vely = sin(launch_angle_degrees)*vel);
      /// Add wind here
      xpos += (windvelx = cos(wind_angle_degrees)*velwind);
      ypos += (windvely = sin(wind_angle_degrees)*velwind);
      /// Add drag here
      velx *= 0.95;
      vely *= 0.95;
      /// Add gravity here
      vely += gravity*dt;
      return DIALOG_OKAY;
   }
   if (explode) {
      explosion.AdvanceAnimationTime(dt);
      exp_timer -= dt;
      if (exp_timer < 0.0) {
         explode = false;
         explosion.ResetAnimation();
         return DIALOG_REMOVE_ME;
      }
   }
   return DIALOG_OKAY;
}



void Projectile::Explode(EagleGraphicsContext* win , EagleImage* game_map) {
   explode = true;
   explosion.ResetAnimation();
   map = game_map;
}




