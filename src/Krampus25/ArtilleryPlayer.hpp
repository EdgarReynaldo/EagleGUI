



#ifndef ArtilleryPlayer_HPP
#define ArtilleryPlayer_HPP


#include "Artillery.hpp"
#include "Turret.hpp"


#include "Eagle/Events.hpp"


class EagleGraphicsContext;


class ArtilleryPlayer {
public :
   Artillery base;
   Turret turret;
   int score;
   bool aim;


   ArtilleryPlayer();

   virtual void Setup()=0;
   virtual void Launch()=0;
   
   
   virtual ~ArtilleryPlayer(){}
   virtual int HandleEvent(EagleEvent e)=0;
   virtual int Update(double dt)=0;
   virtual void DisplayOn(EagleGraphicsContext* win)=0;
};

class HumanPlayer : public ArtilleryPlayer {
public :
   
   HumanPlayer();
   
   virtual void Setup();
   virtual void Launch();
   
   virtual int HandleEvent(EagleEvent e);
   virtual int Update(double dt);
   virtual void DisplayOn(EagleGraphicsContext* win);
};






#endif // ArtilleryPlayer_HPP
