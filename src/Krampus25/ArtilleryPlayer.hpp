



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
   int score;/// Best of 9
   int tourney;/// # of tourneys won (for each best of 9)
   bool aim;
   bool turn;


   ArtilleryPlayer();
   virtual ~ArtilleryPlayer(){}

   virtual void Setup(int x , int y , EagleColor fill , EagleColor draw , double angle);
   virtual void Launch();
   
   
   virtual int HandleEvent(EagleEvent e);
   virtual int Update(double dt);
   virtual void DisplayOn(EagleGraphicsContext* win);
};

class HumanPlayer : public ArtilleryPlayer {
public :
   
   HumanPlayer();
};





class DumbAiPlayer : public ArtilleryPlayer {
public :
   
   int ptarget;
   int ready;
   
   
   
   DumbAiPlayer();
   
   
   virtual int HandleEvent(EagleEvent e);
   virtual int Update(double dt);
   virtual void DisplayOn(EagleGraphicsContext* win);

};





#endif // ArtilleryPlayer_HPP
