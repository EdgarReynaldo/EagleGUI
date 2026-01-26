



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
      aim(false),
      turn(false)
{}



void ArtilleryPlayer::Setup(int x , int y , EagleColor fill , EagleColor draw , double angle) {
   base.Setup(x , y  , new Rectangle(x-20 , y - 20 , 40 , 40) , fill , draw);
   turret.Place(x , y);
   turret.Aim(x + 100*cos(angle) , y - 100*sin(angle));
   turret.Power(25);
   score = 0;
   aim = false;
}



void ArtilleryPlayer::Launch() {
   double angle = atan2(turret.aimy - turret.ypos , turret.aimx - turret.xpos);
   
   base.Launch(angle , turret.aimpower);
   
}



int ArtilleryPlayer::HandleEvent(EagleEvent e) {
   if (!turn) {return DIALOG_OKAY;}
      
   if (e.type == EAGLE_EVENT_MOUSE_AXES) {
      if (aim) {
         turret.Aim(e.mouse.x , e.mouse.y);
         int power = turret.aimpower;
         if (e.mouse.dz) {
            if (input_key_held(EAGLE_KEY_ANY_SHIFT)) {
               power += 5*e.mouse.dz;
            }
            else if (input_key_held(EAGLE_KEY_ANY_CTRL)) {
               power += 10*e.mouse.dz;
            }
            else if (input_key_held(EAGLE_KEY_ANY_ALT)) {
               power += 25*e.mouse.dz;
            }
            else {
               power += e.mouse.dz;
            }
            if (power > 250) {power = 250;}
            if (power < 1) {
               power = 1;
            }
            turret.aimpower = power;
         }
      }
   }   
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
//      if (e.mouse.button == 3) {
//         agame->AddProjectile(new Projectile(turret.xpos , turret.ypos , atan2(turret.aimy - turret.ypos , turret.aimx - turret.xpos) , turret.aimpower));
//      }
   }
   return DIALOG_OKAY;
}



int ArtilleryPlayer::Update(double dt) {
   (void)dt;
//   base.Update(dt);
//   turret.Update(dt);
   return DIALOG_OKAY;
}



void ArtilleryPlayer::DisplayOn(EagleGraphicsContext* win) {
   win->DrawTextString(GetFont("Data/Fonts/Verdana.ttf") , StringPrintF("%i" , score) , base.cx , 50 , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_TOP);
   win->DrawFilledRectangle(base.cx , 100 , 104 , 24 , EagleColor(0,0,0));
   win->DrawRectangle(base.cx , 102 , 104 , 24 , 2.0 , EagleColor(255,255,255));
   win->DrawFilledRectangle(base.cx + 2 , 104 , turret.aimpower*2/5 , 20 , EagleColor(0,255,0));
   win->DrawTextString(GetFont("Data/Fonts/Verdana.ttf") , StringPrintF("%i" , turret.aimpower) , base.cx + 106 , 102 , EagleColor(0,255,0));
   base.DisplayOn(win);
   turret.DisplayOn(win , aim);
}


HumanPlayer::HumanPlayer() :
      ArtilleryPlayer()
{}



/// ********************* dumb ai player       *****************
int DumbAiPlayer::HandleEvent(EagleEvent e) {
   
   (void)e;
   return DIALOG_OKAY;
}



int DumbAiPlayer::Update(double dt) {
   (void)dt;
   return DIALOG_OKAY;
   
   
   
}



void DumbAiPlayer::DisplayOn(EagleGraphicsContext* win) {
   (void)win;
}



