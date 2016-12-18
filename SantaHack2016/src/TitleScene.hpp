


#ifndef TitleScene_HPP
#define TitleScene_HPP



#include "Scene.hpp"

#include "TextDisplay.hpp"


class TitleScene : public Scene {
   
protected :

   EagleGraphicsContext* our_window;

   EagleImage* landscape;

   TextDisplay text_display;
   
   double hold_time;
   double fade_time;
   
   
public :
   
   
   
   virtual bool Init(EagleGraphicsContext* window);
///   virtual bool Init();
   
   virtual void Display(EagleGraphicsContext* win);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);
   
   
};




#endif // TitleScene_HPP

