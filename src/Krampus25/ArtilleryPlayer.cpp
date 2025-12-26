



#include "ArtilleryPlayer.hpp"
#include "Artillery.hpp"
#include "Turret.hpp"
#include "ArtilleryGame.hpp"
#include "Games.hpp"
#include "Projectile.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/Input.hpp"
#include "Eagle/Gui/WidgetBase.hpp"

#include <cmath>


ArtilleryPlayer::ArtilleryPlayer() :
      base(),
      turret(),
      score(0),
      aim(false)
{}



HumanPlayer::HumanPlayer() :
      ArtilleryPlayer()
{}



void HumanPlayer::Setup() {
   base.Setup(50 , 400 , new Rectangle(0 , 0 , 40 , 40) , EagleColor(255,0,255) , EagleColor(0,255,0));
   turret.Place(50 , 400);
   turret.Aim(100 , 400);
   turret.Power(500);
   score = 0;
   aim = false;
}



void HumanPlayer::Launch() {
   double angle = atan2(turret.aimy - turret.ypos , turret.aimx - turret.xpos);
   
   base.Launch(angle , turret.aimpower);
   
}



int HumanPlayer::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (e.mouse.button == 1) {
         aim = !aim;
      }
      else if (e.mouse.button == 3) {
         
      }
   }
   if (e.type == EAGLE_EVENT_MOUSE_AXES) {
      if (aim) {
         turret.Aim(e.mouse.x , e.mouse.y);
      }
      int power = turret.aimpower;
      if (e.mouse.dz) {
         if (input_key_held(EAGLE_KEY_ANY_SHIFT)) {
            power += 5*e.mouse.dz;
         }
         else if (input_key_held(EAGLE_KEY_ANY_CTRL)) {
            power += 25*e.mouse.dz;
         }
         else if (input_key_held(EAGLE_KEY_ANY_ALT)) {
            power += 100*e.mouse.dz;
         }
         else {
            power += e.mouse.dz;
         }
         if (power > 1000) {power = 1000;}
         if (power < 0) {
            power = 0;
         }
         turret.aimpower = power;
      }
   }
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (e.mouse.button == 1) {
         aim = !aim;
      }
      else if (e.mouse.button == 3) {
         agame->AddProjectile(new Projectile(turret.xpos , turret.ypos , atan2(turret.aimy - turret.ypos , turret.aimx - turret.xpos) , turret.aimpower));
      }
   }
   return DIALOG_OKAY;
}



int HumanPlayer::Update(double dt) {
//   base.Update(dt);
//   turret.Update(dt);
   return DIALOG_OKAY;
}



void HumanPlayer::DisplayOn(EagleGraphicsContext* win) {
   base.DisplayOn(win);
   turret.DisplayOn(win);
}



