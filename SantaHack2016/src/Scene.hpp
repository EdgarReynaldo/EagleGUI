




#ifndef Scene_HPP
#define Scene_HPP


#include "Eagle.hpp"



enum SCENE_STATE {
   
   STATE_RUNNING = 0,
   STATE_COMPLETE = 1
   
};

class Scene {
   
protected :
   
public :
   
   virtual ~Scene();
   
   virtual bool Init(EagleGraphicsContext* window)=0;
   virtual void Free(EagleGraphicsContext* window)=0;
   
   virtual void Display(EagleGraphicsContext* window)=0;
   virtual int HandleEvent(EagleEvent ev)=0;
   virtual int Update(double tsec)=0;
   
};



#endif // Scene_HPP

