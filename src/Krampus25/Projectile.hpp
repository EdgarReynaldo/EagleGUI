



#ifndef Projectile_HPP
#define Projectile_HPP

class Projectile {
   double xpos;
   double ypos;
   double theta_degrees;
   double pixels_per_second;
   double power;

   AnimationBase explosion;
   
   void ExplodeOnMap(EagleGraphicsContext* win , EagleImage* game_map;)
};

#endif // Projectile_HPP
