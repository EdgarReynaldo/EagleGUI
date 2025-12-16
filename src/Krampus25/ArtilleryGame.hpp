



#ifndef ArtilleryGame_HPP
#define ArtilleryGame_HPP

#include "Eagle/Events.hpp"
#include "Eagle/GraphicsContext.hpp"

#include <vector>

class ArtilleryPlayer {
   Artillery base;
   
   virtual bool HandleEvent(EagleEvent e)=0;

};


class Projectile;

class ArtilleryGame : public Scene {

   EagleGraphicsContext* window;
   Terrain terrain;
   
   ArtilleryPlayer* p1;
   ArtilleryPlayer* p2;
   std::vector<Projectile*> live_rounds;


   virtual int HandleEvent(EagleEvent e);
   virtual int HandleEvent(EagleEvent e) {
      if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
         return DIALOG_CLOSE;
      }
      return DIALOG_OKAY;
   }
   virtual void Display(EagleGraphicsContext* win);
   virtual void Display(EagleGraphicsContext* win) {
      terrain.DrawOn(win);
      p1->DrawOn(win);
      p2->DrawOn(win);
      for (unsigned int i = 0 ; i < live_rounds.size() ; ++i) {
         live_rounds[i]->DrawOn(win);
      }
   }
   virtual int Update(double dt);
   virtual int Update(double dt) {
      p1->Update(dt);
      p2->Update(dt);
      for (unsigned int i = 0 ; i < live_rounds.size() ; ++i) {
         live_rounds[i]->Update(dt);
      }
   }

};

#endif // ArtilleryGame_HPP
