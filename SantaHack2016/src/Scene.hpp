




#ifndef Scene_HPP
#define Scene_HPP


#include "Eagle.hpp"



enum SCENE_STATE {
   
   SCENE_RUNNING = 0,
   SCENE_COMPLETE = 1
   
};

class Scene {
   
protected :
   
public :
   
   virtual ~Scene() {}
   
   virtual bool Init(EagleGraphicsContext* window)=0;
   
   virtual void Display(EagleGraphicsContext* win)=0;
   virtual int HandleEvent()=0;
   virtual int Update(double tsec)=0;
   
};



#endif // Scene_HPP

