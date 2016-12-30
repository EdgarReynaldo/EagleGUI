



#ifndef Characters_HPP
#define Characters_HPP


#include "Eagle.hpp"



enum DIRECTION {
   DIR_NW = 0,
   DIR_N = 1,
   DIR_NE = 2,
   DIR_W = 3,
   DIR_NONE = 4,
   DIR_E = 5,
   DIR_SW = 6,
   DIR_S = 7,
   DIR_SE = 8
};



class Character {
   
   DIRECTION direction;
   
   float max_vx;
   float max_vy;
   
   float cx;
   float cy;
   
   float nx;
   float ny;
   
   EagleImage* image_160;
   EagleImage* image_64;
   
///   Inventory items;
   
public :
      

   virtual void SetDirection(DIRECTION d);

   virtual void TryToMove(int xdir , int ydir , double dt);
   
   void Draw160(EagleGraphicsContext* win , int xpos , int ypos);
   void Draw64(EagleGraphicsContext* win , int xpos , int ypos);
   void Draw(EagleGraphicsContext* win);

   float CX();
   float CY();
   float NX();
   float NY();
};


class Human : public Character {
   
   EagleImage* image_32;
   EagleImage* image_32bw;
   
public :
   
   void Draw32(EagleGraphicsContext* win , int xpos , int ypos);
   void Draw32BW(EagleGraphicsContext* win , int xpos , int ypos);
   
};

class FunnyBunny : public Character {
   
   
   
};

class Bunny : public Character {
   
   
   EagleImage* images_160[9];
   EagleImage* images_64[9];
   
   EagleImage* hello_bunny160;
   
};


#endif // Characters_HPP

