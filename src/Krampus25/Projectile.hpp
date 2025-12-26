



#ifndef Projectile_HPP
#define Projectile_HPP


#include "Eagle/AnimationBase.hpp"

class EagleGraphicsContext;

class Projectile {
public :
   double startxpos;
   double startypos;
   double launch_angle_degrees;
   double wind_angle_degrees;
   double vel;
   double velwind;
   double expower;

   double xpos;
   double ypos;
   double velx;
   double vely;
   double windvelx;
   double windvely;
   double gravity;
   
   bool explode;
   double exp_timer;
   
   AnimationBase explosion;
   std::vector<EagleImage*> frames;
   
   EagleImage* map;
   
   
   Projectile(double x , double y , double angle_degrees , double power);
   ~Projectile();
   
   void DisplayOn(EagleGraphicsContext* win);
   int Update(double dt);

   void Explode(EagleImage* game_map);



};

#endif // Projectile_HPP
