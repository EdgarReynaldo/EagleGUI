



#ifndef Lightning_HPP
#define Lightning_HPP


#include "Eagle/Position.hpp"
#include "Animation.hpp"



class Runner {
public :
   Pos2D srcpos;
   Pos2D destpos;
   Runner* runprev;
   Runner* runmain;
   Runner* runfork;

   Runner();
   ~Runner();

   void Bolt(Pos2D start , Pos2D stop , double max_leg_length);
   void Free();

   void Draw(EagleGraphicsContext* win , double length);
   
   double Length();
   
};

class LightningBolt : public Animation {
   
///   Pos2D headpos;
///   Pos2D tailpos;
   
   Runner head;
   
   
   void OnSetAnimationPercent() {}

   void OnLoopComplete() {}
   void OnComplete() {}

public :
   
   LightningBolt();
   ~LightningBolt();
   
   void Draw(EagleGraphicsContext* win , int x , int y);
   void Create(int rng_seed , int x1 , int y1 , int x2 , int y2);
   
};

#endif // Lightning_HPP

