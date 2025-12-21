



#ifndef ArtilleryPlayer_HPP
#define ArtilleryPlayer_HPP


#include "Artillery.hpp"



class ArtilleryPlayer {
public :
   Artillery base;
   int score;
   int gamecount;
   virtual ~ArtilleryPlayer();
   virtual int HandleEvent(EagleEvent e)=0;
   virtual int Update(double dt)=0;
   virtual void DisplayOn(EagleGraphicsContext* win)=0;
};


#endif // ArtilleryPlayer_HPP
