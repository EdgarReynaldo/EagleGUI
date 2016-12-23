



#ifndef PresentationScene_HPP
#define PresentationScene_HPP




#include "Scene.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

/**

Edgar Reynaldo

    /**\

  Presents!

*/



class PresentationScene : public Scene {

protected :
      
   Allegro5Image* edgar_image;
   Allegro5Font* edgar_font;
   
   WidgetHandler gui;
   RelativeLayout relative;
   BasicIcon edgar_icon;
   BasicText edgar_text;
   BasicText present_text;
   
   double fade_time;
   double display_time;
   double elapsed_time;
   
public :
   
   PresentationScene();
         
      
      
   virtual bool Init(EagleGraphicsContext* window);
   virtual void Free(EagleGraphicsContext* window);
   
   virtual void Display(EagleGraphicsContext* window);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);
};



#endif // PresentationScene_HPP

