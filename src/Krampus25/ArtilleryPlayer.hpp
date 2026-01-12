



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
   bool turn;


   ArtilleryPlayer();

   virtual void Setup(int x , int y , EagleColor fill , EagleColor draw , double angle)=0;
   virtual void Launch()=0;
   
   
   virtual ~ArtilleryPlayer(){}
   virtual int HandleEvent(EagleEvent e)=0;
   virtual int Update(double dt)=0;
   virtual void DisplayOn(EagleGraphicsContext* win)=0;
};

class HumanPlayer : public ArtilleryPlayer {
public :
   
   HumanPlayer();
   
   virtual void Setup(int x , int y , EagleColor fill , EagleColor draw , double angle);
   virtual void Launch();
   
   virtual int HandleEvent(EagleEvent e);
   virtual int Update(double dt);
   virtual void DisplayOn(EagleGraphicsContext* win);
};






#endif // ArtilleryPlayer_HPP
