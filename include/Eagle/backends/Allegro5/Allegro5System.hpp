

#ifndef Allegro5_System_HPP
#define Allegro5_System_HPP


#include "Eagle/System.hpp"

#include "allegro5/allegro.h"


class Allegro5System : public EagleSystem {

private :
   
   virtual bool PrivateInitializeSystem();

   virtual bool PrivateInitializeImages();
   virtual bool PrivateInitializeFonts();
   virtual bool PrivateInitializeTTFFonts();
   virtual bool PrivateInitializeAudio();
   virtual bool PrivateInitializeShaders();
   virtual bool PrivateInitializePrimitives();

   virtual bool PrivateInstallKeyboard();
   virtual bool PrivateInstallMouse();
   virtual bool PrivateInstallJoystick();
   virtual bool PrivateInstallTouch();
   
   EagleInputHandler*    PrivateCreateInputHandler();
   EagleEventHandler*    PrivateCreateEventHandler(bool delay_events = true);
   EagleTimer*           PrivateCreateTimer();
   EagleGraphicsContext* PrivateCreateGraphicsContext(int width , int height , int flags);
   EagleThread*          PrivateCreateThread(void* (*process)(EagleThread* , void*) , void* data);
   EagleMutex*           PrivateCreateMutex(bool recursive);

public :

   Allegro5System();
   ~Allegro5System();
   
   void Shutdown();

   double GetProgramTime();
   void Rest(double time);

   
   EagleGraphicsContext* GetGraphicsContext(ALLEGRO_DISPLAY* allegro_display);
};

#endif // Allegro5_System_HPP

