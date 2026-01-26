


#include "Games.hpp"
#include "Projectile.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/WidgetBase.hpp"


Projectile::Projectile(double x , double y , double angle_degrees , double power) :
      startxpos(x),
      startypos(y),
      launch_angle_degrees(angle_degrees),
      wind_angle_degrees(0.0),
      vel(power),
      velwind(0.0),
      expower(power),
      xpos(x),
      ypos(y),
      velx(0.0),
      vely(0.0),
      windvelx(0.0),
      windvely(0.0),
      gravity(9.8),/// up is down
      explode(false),
      exp_timer(2.0),
      explosion(),
      frames(),
      map(0)
{
   velx = vel*cos(angle_degrees);
   vely = vel*sin(angle_degrees);
   explosion.Init(4 , 1 , exp_timer);
   frames.resize(4 , 0);
   for (unsigned int i = 0 ; i < 4 ; ++i) {
      frames[i] = expsubimages[i];
   }
}



Projectile::~Projectile() {
   (void)0;
}



void Projectile::DisplayOn(EagleGraphicsContext* win) {
   win->DrawTextString(GetFont("Data/Fonts/Verdana.ttf") , StringPrintF("%3.2lf,%3.2lf" , xpos , ypos) , xpos , ypos , EagleColor(255,255,255),HALIGN_CENTER , VALIGN_CENTER);
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
   std::vector<VSPAN>& train = agame->terrain.spans;
   if (dt > 0.0 && !explode) {
      double dx = dt*(velx + windvelx);
      double dy = dt*(vely + windvely);
      xpos += dx;
      ypos += dy;
      if (xpos < 0.0 || xpos >= train.size()) {
         return DIALOG_REMOVE_ME;
      }
      /// Add drag here
//      velx *= 0.999;
//      vely *= 0.999;
      /// Add gravity here
      vely += gravity*dt;
      vel = sqrt(velx*velx + vely*vely);
      
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



void Projectile::Explode(EagleImage* game_map) {
   explode = true;
   explosion.ResetAnimation();
   map = game_map;
}




