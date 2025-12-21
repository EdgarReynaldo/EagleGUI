



#ifndef Scene_HPP
#define Scene_HPP



#include "Eagle/Events.hpp"
#include "Eagle/GraphicsContext.hpp"

class Scene {
public :
   Scene* next;
   
   Scene() : next(0) {}

   virtual int HandleEvent(EagleEvent e)=0;
   virtual void DisplayOn(EagleGraphicsContext* win)=0;
   virtual int Update(double dt)=0;
};

#endif // Scene_HPP
