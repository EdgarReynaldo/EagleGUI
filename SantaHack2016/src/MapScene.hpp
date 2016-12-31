



#ifndef MapScene_HPP
#define MapScene_HPP



#include "Scene.hpp"

#include "GameMap.hpp"




class MapScene : public Scene {
   
   EagleGraphicsContext* our_window;
   
   GameMap game_map;
   
      
      
      
   double time_elapsed;
   
public :
   
   MapScene();
   MapScene() :
         our_window
   {}
   
   
   
   
   virtual bool Init(EagleGraphicsContext* window);
   virtual void Free(EagleGraphicsContext* window);
   
   virtual void Display(EagleGraphicsContext* window);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);
   
};



#endif // MapScene_HPP

