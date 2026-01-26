



#ifndef Turret_HPP
#define Turret_HPP




class EagleImage;
class EagleGraphicsContext;

class Turret {
public :
   int xpos;
   int ypos;
   int aimx;
   int aimy;
   int aimpower;
   
   
   void Place(int x , int y);
   void Aim(int x , int y);
   void Power(int p);
   void DisplayOn(EagleGraphicsContext* win , bool aim);



};

#endif // Turret_HPP
